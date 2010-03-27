// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unitlib.h>

class CMainDlg : public CDialogImpl<CMainDlg>,
                 public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

		COMMAND_ID_HANDLER(IDC_BTN_CONVERT, OnConvert)
		COMMAND_ID_HANDLER(IDC_BTN_SQRT, OnSqrt)
		COMMAND_ID_HANDLER(IDC_BTN_NEW_RULE, OnNewRule)
		COMMAND_ID_HANDLER(IDC_BTN_LOAD_RULES, OnLoadRules)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CMainDlg)
		DDX_TEXT(IDC_ED_UNIT1, edUnit1)
		DDX_TEXT(IDC_ED_UNIT2, edUnit2)
		DDX_TEXT(IDC_ED_RESULT, edResult)
		DDX_CHECK(IDC_CHK_REDUCE, chkReduce);
		DDX_RADIO(IDC_RD_PLAIN, rdFormat);
	END_DDX_MAP()

	CMainDlg();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnConvert(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSqrt(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNewRule(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLoadRules(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	enum {SETTINGS_HEIGHT = 120};

	bool hasResult;
	bool chkReduce;
	int  rdFormat;
	CString edUnit1, edUnit2, edResult;
	unit_t result;

	void LoadRules(const CString& path);

	void DisplayError();
	void DisplayResult();
	void DisplayText(const CString& text);

	void HasResult(bool has);
};
