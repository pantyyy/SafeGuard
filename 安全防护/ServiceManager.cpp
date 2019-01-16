// ServiceManager.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "ServiceManager.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CServiceManager 对话框

IMPLEMENT_DYNAMIC(CServiceManager, CDialogEx)

CServiceManager::CServiceManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServiceManager::IDD, pParent)
{

}

CServiceManager::~CServiceManager()
{
}

void CServiceManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_service);
}


BEGIN_MESSAGE_MAP(CServiceManager, CDialogEx)
	ON_COMMAND(ID_SERVICE_Close, &CServiceManager::OnServiceClose)
	ON_COMMAND(ID_SERVICE_Start, &CServiceManager::OnServiceStart)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CServiceManager::OnNMRClickList1)
END_MESSAGE_MAP()


// CServiceManager 消息处理程序

void CServiceManager::TraverseService()
{
	//1.获取服务管理器句柄
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hSCM)
	{
		MessageBox(L"打开服务管理器失败!");
	}

	//2.第一次调 , 获取需要的内存信息
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//3.根据获取的内存信息 , 动态的分配内存的大小
	//并使用指针指向首地址
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = NULL;
	pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)malloc(dwSize);
	//动态内存初始化
	memset(pEnumService , 0 , dwSize);

	//4.第二次枚举 , 给动态分配的内存赋值
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (LPBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//5.遍历服务信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		m_service.InsertItem(i, L"");
		//服务名
		CString service_name(pEnumService[i].lpDisplayName);
		m_service.SetItemText(i , 0 , service_name);

		//进程ID
		CString pid;
		pid.Format(L"%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		m_service.SetItemText(i, 1, pid);

		//描述
		CString description(pEnumService[i].lpDisplayName);
		m_service.SetItemText(i, 2, description);

		//当前状态
		int status = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		switch (status)
		{
		case 1:
			m_service.SetItemText(i, 3, L"已停止");
			break;
		case 2:
			m_service.SetItemText(i, 3, L"正在启动");
			break;
		case 3:
			m_service.SetItemText(i, 3, L"正在停止");
			break;
		case 4:
			m_service.SetItemText(i, 3, L"正在运行");
			break;
		case 5:
			m_service.SetItemText(i, 3, L"即将继续");
			break;
		}

	}

}

BOOL CServiceManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//设置对话框名
	CString DlgTitle("服务管理");
	SetWindowText(DlgTitle.GetBuffer());

	m_service.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_service.InsertColumn(0, L"服务名", 0, 200);
	m_service.InsertColumn(1, L"PID", 0, 50);
	m_service.InsertColumn(2, L"描述", 0, 370);
	m_service.InsertColumn(3, L"当前状态", 0, 100);


	TraverseService();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CServiceManager::OnServiceClose()
{
	// TODO:  在此添加命令处理程序代码
	//获取选中的服务名
	POSITION ps;
	int index;
	ps = m_service.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_service.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString service_name = m_service.GetItemText(index, 0);

	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if ((long)hSCM == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"请求的访问被拒绝");
	}
	else if ((long)hSCM == ERROR_DATABASE_DOES_NOT_EXIST)
	{
		MessageBox(L"指定的数据库不存在");
	}

	//获取服务句柄
	SC_HANDLE hService = OpenService(hSCM, service_name, SERVICE_ALL_ACCESS);
	if ((long)hService == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"句柄无权访问该服务");
	}

	SERVICE_STATUS status;
	ControlService(hService, SERVICE_CONTROL_STOP, &status);
	CloseServiceHandle(hSCM);

	m_service.DeleteAllItems();
	TraverseService();
}


void CServiceManager::OnServiceStart()
{
	// TODO:  在此添加命令处理程序代码
	//获取选中的服务名
	POSITION ps;
	int index;
	ps = m_service.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_service.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString service_name = m_service.GetItemText(index, 0);

	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL , NULL , SC_MANAGER_ALL_ACCESS);

	if ((long)hSCM == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"请求的访问被拒绝");
	}
	else if ((long)hSCM == ERROR_DATABASE_DOES_NOT_EXIST)
	{
		MessageBox(L"指定的数据库不存在");
	}

	//获取服务句柄
	SC_HANDLE hService = OpenService(hSCM , service_name , SERVICE_ALL_ACCESS);
	if ((long)hService == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"句柄无权访问该服务");
	}

	//启动服务
	StartService(hService , NULL , NULL);

	CloseServiceHandle(hSCM);
	m_service.DeleteAllItems();
	TraverseService();


}


void CServiceManager::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	//弹出菜单
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Service)); //加载菜单
	HMENU hSub = GetSubMenu(hMenu, 0); //获取子菜单句柄
	CPoint pt;
	GetCursorPos(&pt); //获取坐标
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

}
