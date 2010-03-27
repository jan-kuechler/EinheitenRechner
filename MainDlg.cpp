// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "NewRuleDlg.h"

#include <unitlib.h>


CMainDlg::CMainDlg()
: hasResult(false),
  chkReduce(false), rdFormat(0)
{ }

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	DoDataExchange(false); // initialize DDX

	// title (dynamic version and compiled version of unitlib)
	SetWindowText(CString("EinheitenRechner - ") + ul_get_name() + " (" + UL_VERSION + ")" );

	// disable the sqrt button
	HasResult(false);
	// and give it an icon
	CButton btnSqrt(GetDlgItem(IDC_BTN_SQRT));
	btnSqrt.SetIcon((HICON)LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDI_ROOT), IMAGE_ICON, 16, 16, 0));

	return TRUE;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnConvert(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_PULL);

	unit_t u1, u2;


	if (!ul_parse(edUnit1, &u1)) {
		DisplayError();
		return 0;
	}
	if (!ul_parse(edUnit2, &u2)) {
		DisplayError();
		return 0;
	}
	if (!ul_copy(&result, &u1)) {
		DisplayError();
		return 0;
	}
	if (!ul_inverse(&u2)) {
		DisplayError();
		return 0;
	}
	if (!ul_combine(&result, &u2)) {
		DisplayError();
		return 0;
	}

	DisplayResult();
	return 0;
}

LRESULT CMainDlg::OnSqrt(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!ul_sqrt(&result))
		DisplayError();
	else
		DisplayResult();
	return 0;
}

LRESULT CMainDlg::OnNewRule(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CNewRuleDlg dlg;

	if (dlg.DoModal() == IDOK) {
		if (!ul_parse_rule(dlg.GetRule())) {
			DisplayError();
		}
		else {
			DisplayText(_T("Regel geladen"));
		}
	}
	return 0;
}

LRESULT CMainDlg::OnLoadRules(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK) {
		LoadRules(dlg.m_szFileName);
	}

	return 0;
}

void CMainDlg::LoadRules(const CString& path)
{
	if (!ul_load_rules(path))
		DisplayError();
	else
		DisplayText(path + " geladen");
}

void CMainDlg::DisplayError()
{
	edResult = ul_error();
	DoDataExchange(DDX_PUSH);
	HasResult(false);
}

void CMainDlg::DisplayResult()
{
	enum { BUFLEN = 2048 };
	static char buffer[BUFLEN ];

	int fops = 0;
	if (chkReduce)
		fops |= UL_FOP_REDUCE;
	
	if (!ul_snprint(buffer, BUFLEN , &result, static_cast<ul_format_t>(rdFormat), fops)) {
		DisplayError();
		return;
	}

	edResult = buffer;
	DoDataExchange(DDX_PUSH);
	HasResult(true);
}

void CMainDlg::DisplayText(const CString& text)
{
	edResult = text;
	DoDataExchange(DDX_PUSH);
	HasResult(false);
}

void CMainDlg::HasResult(bool has)
{
	hasResult = has;
	CWindow btnSqrt(GetDlgItem(IDC_BTN_SQRT));
	btnSqrt.EnableWindow(hasResult);
}
