// EinheitenRechner.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// MainApp:
// See EinheitenRechner.cpp for the implementation of this class
//

class MainApp : public CWinApp
{
public:
	MainApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	enum FormatStyle
	{
		FmtPlain, FmtLatexInline, FmtLatexFrac
	};

	const char *Convert(const char *unit1, const char *unit2, FormatStyle fmt, bool reduce) const;
	void LoadRules(const char *path);

private:
	void TryLoadRules();
};

extern MainApp theApp;