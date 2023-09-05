
// childDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "child.h"
#include "childDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CchildDlg 对话框



CchildDlg::CchildDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHILD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editParent);
}

BEGIN_MESSAGE_MAP(CchildDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CchildDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CchildDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CchildDlg::OnBnClickedButton2)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON3, &CchildDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CchildDlg::OnBnClickedButton4)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CchildDlg 消息处理程序

BOOL CchildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(L"IPCAMERA_CHILD");
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW /*| WS_EX_TRANSPARENT| WS_EX_LAYERED*/);
	ModifyStyle(0, WS_CLIPSIBLINGS);

	::SetLayeredWindowAttributes(m_hWnd, 0, 254, LWA_ALPHA);

	MoveWindow(0, 0, 400, 300);

	SetTimer(2000, 2000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchildDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchildDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rect;
	GetClientRect(&rect);

	COLORREF clr(0X00FF00);
	CBrush b(clr);
	dc.FillRect(&rect, &b);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchildDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchildDlg::OnBnClickedCancel()
{
	SetParent(nullptr);
	ModifyStyle(WS_CHILD, WS_POPUP | WS_BORDER | WS_DLGFRAME, NULL);
}


#include <string>
void CchildDlg::OnBnClickedButton1()
{
	CString text;
	m_editParent.GetWindowTextW(text);

	HWND parent = (HWND)std::stoi(text.GetBuffer());
	if (IsWindow(parent)) {

		ModifyStyle(WS_POPUP | WS_BORDER | WS_DLGFRAME, WS_CHILD, NULL);
		::SetParent(m_hWnd, parent);

		RECT rc;
		::GetClientRect(parent, &rc);

		MoveWindow(10, 30, rc.right - 20, rc.bottom - 80);
	}
}


void CchildDlg::OnBnClickedButton2()
{
	exit(1);
}


BOOL CchildDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CchildDlg::OnBnClickedButton3()
{
	CRect rc;
	GetWindowRect(&rc);

	rc.left -= 20;
	rc.right -= 20;

	MoveWindow(&rc);
}


void CchildDlg::OnBnClickedButton4()
{
	CRect rc;
	GetWindowRect(&rc);

	rc.left += 20;
	rc.right += 20;

	MoveWindow(&rc);
}


void CchildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsWindowVisible()) {
		ShowWindow(SW_HIDE);
	}
	else {
		ShowWindow(SW_SHOW);
	}

	CDialogEx::OnTimer(nIDEvent);
}
