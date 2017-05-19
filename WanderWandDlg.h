
// WanderWandDlg.h : header file
//

#pragma once
#include <Windows.h>
#include <Vfw.h>
#include "Options.h"
#include "VisualInterpreter.h"

// CWanderWandDlg dialog
class CWanderWandDlg : public CDialogEx
{
protected:
	HWND h_MFCWindow;


#pragma region Default


// Construction
public:
	CWanderWandDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WANDERWAND_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

#pragma endregion

	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
};
