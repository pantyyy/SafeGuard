
// 安全防护Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "安全防护Dlg.h"
#include "afxdialogex.h"
#include "Pe.h"
#include "TaskManager.h"
#include "TrashClean.h"
#include "KillVirus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// C安全防护Dlg 对话框



C安全防护Dlg::C安全防护Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C安全防护Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C安全防护Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C安全防护Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PE, &C安全防护Dlg::OnBnClickedButtonPe)
	ON_BN_CLICKED(IDC_BUTTON_TaskManager, &C安全防护Dlg::OnBnClickedButtonTaskmanager)
	ON_BN_CLICKED(IDC_BUTTON_CleanTrash, &C安全防护Dlg::OnBnClickedButtonCleantrash)
	ON_BN_CLICKED(IDC_BUTTON_KillVirus, &C安全防护Dlg::OnBnClickedButtonKillvirus)
	ON_COMMAND(ID_Boss_On, &C安全防护Dlg::OnBossOn)
	ON_COMMAND(ID_Boss_Off, &C安全防护Dlg::OnBossOff)
	ON_COMMAND(ID_ClockWin, &C安全防护Dlg::OnClockwin)
	ON_COMMAND(ID_Shutdown, &C安全防护Dlg::OnShutdown)
	ON_COMMAND(ID_Reboot, &C安全防护Dlg::OnReboot)
	ON_COMMAND(ID_Hibernate, &C安全防护Dlg::OnHibernate)
END_MESSAGE_MAP()


// C安全防护Dlg 消息处理程序

BOOL C安全防护Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码
	boss_menu.LoadMenu(IDR_MENU_Boss_Key);
	SetMenu(&boss_menu);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C安全防护Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C安全防护Dlg::OnPaint()
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
HCURSOR C安全防护Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C安全防护Dlg::OnBnClickedButtonPe()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建PE对话框
	CPe* pe = new CPe();
	pe->Create(IDD_DIALOG_PE);
	pe->ShowWindow(SW_SHOW);
}


void C安全防护Dlg::OnBnClickedButtonTaskmanager()
{
	// TODO:  在此添加控件通知处理程序代码
	CTaskManager* TaskManager = new CTaskManager();
	TaskManager->Create(IDD_DIALOG_TaskManager);
	TaskManager->ShowWindow(SW_SHOW);
}


void C安全防护Dlg::OnBnClickedButtonCleantrash()
{
	// TODO:  在此添加控件通知处理程序代码
	CTrashClean* TrashClean = new CTrashClean();
	TrashClean->Create(IDD_DIALOG_Trash_Clean);
	TrashClean->ShowWindow(SW_SHOW);
}




void C安全防护Dlg::OnBnClickedButtonKillvirus()
{
	// TODO:  在此添加控件通知处理程序代码
	CKillVirus* KillVirus = new CKillVirus();
	KillVirus->Create(IDD_DIALOG_Kill_Virus);
	KillVirus->ShowWindow(SW_SHOW);
}


void C安全防护Dlg::OnBossOn()
{
	// TODO:  在此添加命令处理程序代码
	//MessageBox(L"开启老板键");
	//1.注册全局热键
	::RegisterHotKey(this->GetSafeHwnd(), /*获取当前窗口的句柄*/
		0Xa001, /*自定义快捷键的标识*/
		MOD_CONTROL | MOD_SHIFT, /*同时按下ctrl , shift*/
		'K');

	MessageBox(L"老板键开启成功！");

}


void C安全防护Dlg::OnBossOff()
{
	// TODO:  在此添加命令处理程序代码
	::UnregisterHotKey(this->GetSafeHwnd(), 0Xa001);
	MessageBox(L"老板键关闭成功！");
}


BOOL C安全防护Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	//判断是否是全局热键 , 同时还是我们注册的键
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0xa001))
	{
		//隐藏窗口
		if (m_IsWindowHide == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		else{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void C安全防护Dlg::OnClockwin()
{
	// TODO:  在此添加命令处理程序代码
	LockWorkStation();
}


void C安全防护Dlg::OnShutdown()
{
	// TODO:  在此添加命令处理程序代码
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 1);
}


void C安全防护Dlg::OnReboot()
{
	// TODO:  在此添加命令处理程序代码
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 1);
}


void C安全防护Dlg::OnHibernate()
{
	// TODO:  在此添加命令处理程序代码
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 1);
	LockWorkStation();
}
