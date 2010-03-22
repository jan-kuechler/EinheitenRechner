// ManDlg.h : header file
//

#pragma once
#include "afxwin.h"


// MainDlg dialog
class MainDlg : public CDialog
{
// Construction
public:
	MainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EINHEITENRECHNER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit edUnit2;
	CEdit edUnit1;
	CEdit edResult;
	CButton chkReduce;
	CButton rdFmt;

	MainApp::FormatStyle GetFormatStyle() const;
public:
	afx_msg void OnBnClickedBtnGo();

	afx_msg void OnBnClickedBtnRules();

private:
	void DisplayResult();
	void DisplayError();
	void ResetResultDisplay();
public:
	afx_msg void OnBnClickedBtnSqrt();
};
