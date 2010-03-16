// EinheitenRechner.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EinheitenRechner.h"
#include "ManDlg.h"

#include <stdio.h>
#include <unitlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const char *RULE_FILE = "rules";
static const unsigned int BUFFER_SIZE = 2048;

// MainApp

BEGIN_MESSAGE_MAP(MainApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// MainApp construction

MainApp::MainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only MainApp object

MainApp theApp;


// MainApp initialization

BOOL MainApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	
	//SetRegistryKey(_T("KuchenEinheitenRechner"));

	
	// Initialization

	//ul_debugout("ul_debug.log", false);
	//ul_debugging(true);

	if (!ul_init()) {
		AfxMessageBox(CString("Error: Failed to initialize unitlib\n") + ul_error());
		return false;
	}

	TryLoadRules();

	MainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	ul_quit();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void MainApp::TryLoadRules()
{
	FILE *f = fopen(RULE_FILE, "r");
	if (f) {
		fclose(f);

		if (!ul_load_rules(RULE_FILE)) {
			AfxMessageBox(CString("Error: Failed to load default rules.\n") + ul_error());
		}
	}
}

static ul_format_t format_cast(MainApp::FormatStyle style)
{
	switch (style) {
	case MainApp::FmtPlain: return UL_FMT_PLAIN;
	case MainApp::FmtLatexInline: return UL_FMT_LATEX_INLINE;
	case MainApp::FmtLatexFrac: return UL_FMT_LATEX_FRAC;
	}
	return UL_FMT_PLAIN;
}

void MainApp::LoadRules(const char *path)
{
	if (!ul_load_rules(path)) {
		AfxMessageBox(CString("Failed to load file: ") + path + "\n" + ul_error());
	}
}

const char *MainApp::Convert(const char *unit1, const char *unit2, FormatStyle fmt, bool reduce) const
{
	static char buffer[BUFFER_SIZE];
	unit_t u1, u2;

	if (!ul_parse(unit1, &u1))
		return ul_error();
	if (!ul_parse(unit2, &u2))
		return ul_error();
	if (!ul_inverse(&u2))
		return ul_error();
	if (!ul_combine(&u1, &u2))
		return ul_error();

	int fop = 0;
	if (reduce)
		fop |= UL_FOP_REDUCE;

	if (!ul_snprint(buffer, BUFFER_SIZE, &u1, format_cast(fmt), fop))
		return ul_error();

	return buffer;
}
