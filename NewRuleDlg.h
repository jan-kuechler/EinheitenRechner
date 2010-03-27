
#pragma once

class CNewRuleDlg : public CDialogImpl<CNewRuleDlg>,
                    public CWinDataExchange<CNewRuleDlg>
{
public:
	enum { IDD = IDD_NEW_RULE };

	BEGIN_MSG_MAP(CNewRuleDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CNewRuleDlg)
		DDX_TEXT(IDC_ED_RULE, edRule)
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CString GetRule() const
	{
		return edRule;
	}
private:
	CString edRule;
};
