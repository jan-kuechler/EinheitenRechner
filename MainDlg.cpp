// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EinheitenRechner.h"
#include "ManDlg.h"

#include <unitlib.h>
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	BOOL rs = CDialog::OnInitDialog();

	CString ulv = "Unitlib: ";
	ulv += ul_get_version();
	ulv += " (";
	ulv += UL_VERSION;
	ulv += ")";

	SetDlgItemText(IDC_VERSION, ulv);
	return rs;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// MainDlg dialog




MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_UNIT1, edUnit1);
	DDX_Control(pDX, IDC_ED_UNIT2, edUnit2);
	DDX_Control(pDX, IDC_ED_RESULT, edResult);
	DDX_Control(pDX, IDC_CHK_REDUCE, chkReduce);
	//DDX_Radio(pDX, IDC_RD_FMT_PLAIN, rdFormat); // added manually
	DDX_Control(pDX, IDC_RD_FMT_PLAIN, rdFmt);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GO, &MainDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_RULES, &MainDlg::OnBnClickedBtnRules)
	ON_BN_CLICKED(IDC_BTN_SQRT, &MainDlg::OnBnClickedBtnSqrt)
END_MESSAGE_MAP()


// MainDlg message handlers

BOOL MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	// TODO: Add extra initialization here

	CButton *defaultRadio = static_cast<CButton*>(GetDlgItem(IDC_RD_FMT_PLAIN));
	defaultRadio->SetCheck(true);

	SetWindowText(CString("Einheiten Rechner - ") + ul_get_name());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void MainDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


MainApp::FormatStyle MainDlg::GetFormatStyle() const
{
	CButton *rdFmtPlain = static_cast<CButton*>(GetDlgItem(IDC_RD_FMT_PLAIN));
	CButton *rdFmtLatexInline = static_cast<CButton*>(GetDlgItem(IDC_RD_FMT_LATEX_INLINE));
	CButton *rdFmtLatexFrac = static_cast<CButton*>(GetDlgItem(IDC_RD_FMT_LATEX_FRAC));

	if (rdFmtPlain->GetCheck())
		return MainApp::FmtPlain;
	if (rdFmtLatexInline->GetCheck())
		return MainApp::FmtLatexInline;
	if (rdFmtLatexFrac->GetCheck())
		return MainApp::FmtLatexFrac;

	return MainApp::FmtPlain;
}

void MainDlg::DisplayError()
{
	edResult.SetWindowText(theApp.GetError());
}

void MainDlg::ResetResultDisplay()
{
	edResult.SetWindowText("");
}

void MainDlg::DisplayResult()
{
	const char *result = theApp.Format(GetFormatStyle(), chkReduce.GetCheck());
	if (!result) {
		DisplayError();
		return;
	}
	ResetResultDisplay();
	edResult.SetWindowText(result);
}

void MainDlg::OnBnClickedBtnGo()
{
	enum { BUFSIZE = 1024 };
	char unit1[BUFSIZE];
	char unit2[BUFSIZE];

	edUnit1.GetWindowText(unit1, BUFSIZE);
	edUnit2.GetWindowText(unit2, BUFSIZE);

	if (!theApp.SetUnits(unit1, unit2)) {
		DisplayError();
		return;
	}
	DisplayResult();
}

void MainDlg::OnBnClickedBtnRules()
{
	CFileDialog openDlg(true);

	if (openDlg.DoModal() == IDOK)
		theApp.LoadRules(openDlg.GetPathName());
}

void MainDlg::OnBnClickedBtnSqrt()
{
	if (theApp.HasResult()) {
		if (!theApp.Sqrt())	
			DisplayError();
		else
			DisplayResult();
	}
}
