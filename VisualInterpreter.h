#pragma once
#include "WanderWandDlg.h"
#include "CustomContainers.h"

#ifndef VINTERP
#define VINTERP

	struct Controller
	{
		v3i wandCursor;
		v3i handCursor;
	};

	class VisualInterpreter
	{

	#pragma region Variables

	public:
		RECT wnd;
		Controller Cursors;
		int hb, ht, hl, hr;
		int wb, wt, wl, wr;

	#pragma endregion

	#pragma region Methods

	public:
		VisualInterpreter();
		~VisualInterpreter();

		void TraceCursor(int x, int y, BYTE r, BYTE b, BOOL IsFrameEnded);
		// process a frame recieved from camera with procFn function
		void CamCaptureProcessing(LPVIDEOHDR *lpVHdr);

	protected:
		void procFn(BYTE *r, BYTE *g, BYTE *b);
		void ConvertRgbToYuy2(BYTE *TwoPixelsRGB, BYTE *lpData);

	private:
		int clip(int x);
		

	#pragma endregion

	};

#endif

