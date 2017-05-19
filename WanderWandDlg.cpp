
// WanderWandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WanderWand.h"
#include "WanderWandDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


VisualInterpreter vi;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWanderWandDlg dialog



CWanderWandDlg::CWanderWandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WANDERWAND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWanderWandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWanderWandDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CWanderWandDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWanderWandDlg message handlers

LRESULT CALLBACK cbFunc(HWND hWnd, LPVIDEOHDR lpVHdr) {

	vi.CamCaptureProcessing( &lpVHdr );

	return (LRESULT)TRUE;
}

BOOL CWanderWandDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 330, 330, ImageX, ImageY, SWP_NOZORDER);

	//HWND hwndParent = this->GetSafeHwnd();
	/*h_MFCWindow = capCreateCaptureWindow(L"Lel",
		WS_CHILD | WS_VISIBLE,
		10, 10, ImageX, ImageY,
		this->m_hWnd, 1);*/

	h_MFCWindow = capCreateCaptureWindow(L"handle", WS_CHILD | WS_VISIBLE,
		0, 0, ImageX, ImageY,
		this->m_hWnd, 1);


	// connect the driver with the webcam
	int xx = capDriverConnect(h_MFCWindow, 0);
	if (xx == false)
	{
		MessageBox(L"No camera found ", 0, 0);
		return false;
	}
	capDlgVideoFormat(h_MFCWindow);

	capSetCallbackOnFrame(h_MFCWindow, cbFunc);

	//----------- Set the Timer to grab new images -----------
	SetTimer(1, 40, 0);
	
	GetClientRect(&(vi.wnd));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWanderWandDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWanderWandDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWanderWandDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWanderWandDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		capGrabFrame(h_MFCWindow);
	}

	CDialog::OnTimer(nIDEvent);
}


void CWanderWandDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
