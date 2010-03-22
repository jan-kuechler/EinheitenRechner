// EinheitenRechner.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <unitlib.h>

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

	void LoadRules(const char *path);

	bool SetUnits(const char *unit1, const char *unit2);
	const char *Format(FormatStyle fmt, bool reduce) const;
	bool Sqrt();
	const char *GetError() const;
	bool HasResult() const
	{
		return hasResult;
	}

private:
	unit_t result;
	mutable const char *lastErr;
	bool hasResult;

	void TryLoadRules();
};

extern MainApp theApp;