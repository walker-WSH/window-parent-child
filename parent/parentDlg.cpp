
// parentDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "parent.h"
#include "parentDlg.h"
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


// CparentDlg 对话框



CparentDlg::CparentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnRun);
}

BEGIN_MESSAGE_MAP(CparentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CparentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_AS_CHILD, &CparentDlg::OnBnClickedButtonAsChild)
	ON_BN_CLICKED(IDC_BUTTON_AS_POPUP, &CparentDlg::OnBnClickedButtonAsPopup)
END_MESSAGE_MAP()


// CparentDlg 消息处理程序
#include <string>
BOOL CparentDlg::OnInitDialog()
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
	ModifyStyle(0, WS_CLIPCHILDREN);

	std::wstring wstr = std::to_wstring((long long)m_hWnd);
	SetWindowText(wstr.c_str());


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CparentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CparentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CparentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include <Windows.h>
#include <shlwapi.h>
#include <string>

#pragma comment(lib, "Shlwapi.lib")

HANDLE child_process = 0;

std::wstring GetFileDirectory(HINSTANCE hInstance = nullptr)
{
	wchar_t dir[MAX_PATH] = {};
	GetModuleFileNameW(hInstance, dir, MAX_PATH);
	PathRemoveFileSpecW(dir); // 处理后末尾不包含'\'
	return std::wstring(dir) + std::wstring(L"\\");
}

void CparentDlg::OnBnClickedOk()
{
	if (child_process) {
		auto ret = WaitForSingleObject(child_process, 0);
		if (ret == WAIT_OBJECT_0) {
			DWORD exit = 0;
			GetExitCodeProcess(child_process, &exit);

			CString str;
			str.Format(L"exit code: %u", exit);
			MessageBox(str, str, 0);

			CloseHandle(child_process);
			child_process = 0;
		}
		else {
			return;
		}
	}

	auto dir = GetFileDirectory();
	dir += L"child.exe";

	//auto param = L"cmd";
	//ShellExecute(nullptr, L"open", dir.c_str(), param, nullptr, SW_HIDE);

	PROCESS_INFORMATION pi = {};
	STARTUPINFO si = {};
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_SHOW;
	BOOL bOK = CreateProcess(dir.c_str(), nullptr, nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
	if (bOK)
	{
		CloseHandle(pi.hThread);
		child_process = pi.hProcess;
		//CloseHandle(pi.hProcess);
	}
}

#include <thread>

void thread_func(HWND child, HWND parent, RECT rc) {

	::SetParent(child, parent); // 先增加POPUP属性，再调用setparent
	MoveWindow(child, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
}

void CparentDlg::OnBnClickedButtonAsChild()
{
	if (!m_pChildWnd || !IsWindow(m_pChildWnd->GetSafeHwnd()))
	{
		m_pChildWnd = FindWindow(nullptr, L"IPCAMERA_CHILD");
	}

	if (m_pChildWnd && IsWindow(m_pChildWnd->GetSafeHwnd()))
	{
		m_pChildWnd->ModifyStyle(WS_POPUP | WS_BORDER | WS_DLGFRAME, WS_CHILD, 0);

		CRect rc;
		GetClientRect(&rc);
		rc.left += 20;
		rc.right -= 20;
		rc.top += 20;
		rc.bottom -= 80;

		std::thread(thread_func, m_pChildWnd->GetSafeHwnd(), m_hWnd, rc).detach();
	}
}


void CparentDlg::OnBnClickedButtonAsPopup()
{
	if (m_pChildWnd && IsWindow(m_pChildWnd->GetSafeHwnd()))
	{
		m_pChildWnd->SetParent(nullptr); // 先调用函数，再添加POPUP
		m_pChildWnd->ModifyStyle(WS_CHILD, WS_POPUP | WS_BORDER | WS_DLGFRAME, 0);
	}
}
