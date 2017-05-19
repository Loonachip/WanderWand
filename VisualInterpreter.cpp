#include "stdafx.h"
#include "VisualInterpreter.h"


VisualInterpreter::VisualInterpreter()
{
	Cursors.handCursor = { 0, 0, 0 };
	Cursors.wandCursor = { 0, 0, 0 };
	hb = 0; ht = 255; hl = 255; hr = 0;
	wb = 0; wt = 255; wl = 255; wr = 0;
}

VisualInterpreter::~VisualInterpreter()
{
}


void VisualInterpreter::TraceCursor(int x, int y, BYTE r, BYTE b, BOOL IsFrameEnded)
{
	if (IsFrameEnded) {
		Cursors.handCursor = { (hl + hr) / 2, (ht + hb) / 2, 0 };
		Cursors.wandCursor = { (wl + wr) / 2, (wt + wb) / 2, 0 };

		hb = 0; ht = 255; hl = 255; hr = 0;
		wb = 0; wt = 255; wl = 255; wr = 0;
		return;
	}

	// for wand cursor
	if (r) {
		wt = min(wt, y);
		wb = max(wb, y);
		wl = min(wl, x);
		wr = max(wr, x);
	}

	// for hand cursor
	if (b) {
		ht = min(ht, y);
		hb = max(hb, y);
		hl = min(hl, x);
		hr = max(hr, x);
	}

}

void VisualInterpreter::procFn(BYTE *r, BYTE *g, BYTE *b)
{
	if (BthreshH > *b && *b > BthreshL) {
		*b = 225;
	}
	else {
		*b = 0;
	}

	if (RthreshH > *r && *r > RthreshL) {
		*r = 225;
	}
	else {
		*r = 0;
	}

	if (*r && *b) {
		*r = 0;
		*b = 0;
	}

	//*b = 0;
	//*r = 255;

}

void VisualInterpreter::CamCaptureProcessing(LPVIDEOHDR * lpVHdr)
{
	/*-------- YUY2 format ----------
		Y=R+G+B=luminance=monochrome
		U=R-Y
		V=B-Y                 !
			Macropixel: Y0 U0 Y1 V0
			Macropixel == 2 pixels!
	-------------------------------*/

	BYTE *YUYvec = (*lpVHdr)->lpData;
	BYTE *TwoPixelsRGB = (BYTE *)malloc(sizeof(BYTE) * 6);

	int YUYlength = (*lpVHdr)->dwBufferLength;
	int iRGB;
	int iYUY;

	int y0, u0, y1, v0;
	int c, d, e;

	int x=0, y=0;


	// Reach RGB values from YUY2 (4:2:2)
	// and work on them with procFn
	for (int i = 0; i < YUYlength; i++) {
		y0 = YUYvec[i];
		u0 = YUYvec[i + 1];
		y1 = YUYvec[i + 2];
		v0 = YUYvec[i + 3];
		c = y0 - 16;
		d = u0 - 128;
		e = v0 - 128;
		iRGB = i * 6;
		iYUY = i * 4;

		// Get R G B values
			/*B*/ TwoPixelsRGB[2] = (298 * c + 516 * d + 128) >> 8;
			/*G*/ TwoPixelsRGB[1] = (298 * c - 100 * d - 208 * e + 128) >> 8; //TwoPixelsRGB[1] = 0; 
			/*R*/ TwoPixelsRGB[0] = (298 * c + 409 * e + 128) >> 8;
		// Work on values
			//procFn(&(TwoPixelsRGB[0]), &(TwoPixelsRGB[1]), &(TwoPixelsRGB[2]));
		// count coords
			if (x < ImageX) x++;
			else { x = 0; y++; }
		// sum coords to find avg at last
			TraceCursor(x, y, TwoPixelsRGB[0], TwoPixelsRGB[2], false);


		// Get next R G B values
			c = y1 - 16;
			/*B*/ TwoPixelsRGB[5] = (298 * c + 516 * d + 128) >> 8;
			/*G*/ TwoPixelsRGB[4] = (298 * c - 100 * d - 208 * e + 128) >> 8; //TwoPixelsRGB[4] = 0; 
			/*R*/ TwoPixelsRGB[3] = (298 * c + 409 * e + 128) >> 8;
		// Work on values
			//procFn(&(TwoPixelsRGB[3]), &(TwoPixelsRGB[4]), &(TwoPixelsRGB[5]));
		// count coords
			if (x < ImageX) x++;
			else { x = 0; y++; }
		// sum coords to find avg at last
			TraceCursor(x, y, TwoPixelsRGB[3], TwoPixelsRGB[5], false);

		// Convert Pixels back to Macropixel
		ConvertRgbToYuy2(TwoPixelsRGB, &(YUYvec[i]));
	}

	TraceCursor(0, 0, 0, 0, true);

	free(TwoPixelsRGB);
}

void VisualInterpreter::ConvertRgbToYuy2(BYTE *TwoPixelsRGB, BYTE *Macropixel)
{
	/*-------- YUY2 format ----------
		Y=R+G+B=luminance=monochrome
		U=R-Y
		V=B-Y                 !
			Macropixel: Y0 U0 Y1 V0
			Macropixel == 2 pixels!
	-------------------------------*/
	
	int r1 = TwoPixelsRGB[0],
		g1 = TwoPixelsRGB[1],
		b1 = TwoPixelsRGB[2],
		r2 = TwoPixelsRGB[3],
		g2 = TwoPixelsRGB[4],
		b2 = TwoPixelsRGB[5];

	
	/*Y0*/ //Macropixel[0] = (clip(66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16;
	/*U0*/ //Macropixel[1] = (clip(-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128;
	/*Y1*/ //Macropixel[2] = (clip(66 * TwoPixelsRGB[3] + 129 * TwoPixelsRGB[4] + 25 * TwoPixelsRGB[5] + 128) >> 8) + 16;
	/*V0*/ //Macropixel[3] = (clip(112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128;
	
	/*Y0*/ Macropixel[0] = (clip(66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16;
	/*U0*/ Macropixel[1] = (clip(-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128;
	/*V0*/ Macropixel[3] = (clip(112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128;

	/*Y1*/ Macropixel[2] = (clip(66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16;
	

}

int VisualInterpreter::clip(int x)
{
	return ((255 < x) ? x : ((255 > 0) ? 0 : 255));
}

