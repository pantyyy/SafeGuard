// TaskManager.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "TaskManager.h"
#include "afxdialogex.h"
#include "Thread.h"
#include "Module.h"
#include "MemoryManager.h"
#include "ServiceManager.h"
#include "Unstall.h"
#include<windows.h>
#include<TlHelp32.h>
#include<vector>
using std::vector;


//获取所有进程对象
vector<PROCESSENTRY32> GetAllProcess()
{
	vector<PROCESSENTRY32> process_list;
	//1.创建进程快照
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	//2.进程结构体 , 用来保存遍历到的每一个进程
	PROCESSENTRY32 process_info = { sizeof(PROCESSENTRY32) };

	//3.尝试获取快照中的第一个进程 , 判断是否成功
	if (Process32First(snapshot, &process_info))
	{
		//输入获取到的进程信息
		do
		{
			process_list.push_back(process_info);

		} while (Process32Next(snapshot, &process_info));//获取下一个进程信息
	}

	return process_list;
}



// CTaskManager 对话框

IMPLEMENT_DYNAMIC(CTaskManager, CDialogEx)

CTaskManager::CTaskManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskManager::IDD, pParent)
{

}

CTaskManager::~CTaskManager()
{
}

void CTaskManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Process);
}


BEGIN_MESSAGE_MAP(CTaskManager, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTaskManager::OnNMRClickList1)
	ON_COMMAND(ID_PROCESS_THREAD, &CTaskManager::OnProcessThread)
	ON_COMMAND(ID_PROCESS_MODEL, &CTaskManager::OnProcessModel)
	ON_COMMAND(ID_PROCESS_TERMINATEPROCESS, &CTaskManager::OnProcessTerminateprocess)
	ON_BN_CLICKED(IDC_BUTTON_ShowWin, &CTaskManager::OnBnClickedButtonShowwin)
	ON_BN_CLICKED(IDC_BUTTON_Memory_Control, &CTaskManager::OnBnClickedButtonMemoryControl)
	ON_BN_CLICKED(IDC_BUTTON_Service, &CTaskManager::OnBnClickedButtonService)
	ON_BN_CLICKED(IDC_BUTTON_Uninstall, &CTaskManager::OnBnClickedButtonUninstall)
	ON_COMMAND(ID_PROCESS_Protect, &CTaskManager::OnProcessProtect)
END_MESSAGE_MAP()


// CTaskManager 消息处理程序


BOOL CTaskManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框名
	CString DlgTitle("任务管理器");
	SetWindowText(DlgTitle.GetBuffer());

	//设置表头
	m_Process.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Process.InsertColumn(0, L"名称", 0, 200);
	m_Process.InsertColumn(1, L"进程ID", 0, 80);
	m_Process.InsertColumn(2, L"父进程ID", 0, 80);
	m_Process.InsertColumn(3, L"线程数", 0, 80);
	m_Process.InsertColumn(4, L"位数", 0, 80);
	m_Process.InsertColumn(5, L"路径", 0, 500);

	//获取进程列表
	vector<PROCESSENTRY32> process_list = GetAllProcess();

	//遍历进程列表
	for (int i = 0; i < process_list.size(); i++)
	{
		int j = 0;
		//进程名
		m_Process.InsertItem(i, process_list[i].szExeFile);

		//进程ID
		CString c_id;
		c_id.Format(TEXT("%d"), process_list[i].th32ProcessID);
		m_Process.SetItemText(i, ++j, c_id);

		//父进程ID
		CString p_id;
		p_id.Format(TEXT("%d"), process_list[i].th32ParentProcessID);
		m_Process.SetItemText(i, ++j, p_id);

		//线程数
		CString count_thread;
		count_thread.Format(TEXT("%d"), process_list[i].cntThreads);
		m_Process.SetItemText(i, ++j, count_thread);

		//进程完整路径
		//1.获取进程句柄
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_list[i].th32ProcessID);
		//2.创建数组保存路径
		DWORD length = MAX_PATH;
		WCHAR path[MAX_PATH] = { 0 };
		//3.输入完整路径
		QueryFullProcessImageName(process, 0, path, &length);


		BOOL Is32 = FALSE;
		//64 - bit process on 64 - bit Windows : FALSE
		//32 - bit process on 64 - bit Windows : TRUE
		IsWow64Process(process, &Is32);
		if (Is32)
			m_Process.SetItemText(i, ++j, L"32位");
		else
			m_Process.SetItemText(i, ++j, L"64位");

		m_Process.SetItemText(i, ++j, path);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CTaskManager::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	//弹出菜单
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Process)); //加载菜单
	HMENU hSub = GetSubMenu(hMenu, 0); //获取子菜单句柄
	CPoint pt;
	GetCursorPos(&pt); //获取坐标
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);


	*pResult = 0;
}


void CTaskManager::OnProcessThread()
{
	// TODO:  在此添加命令处理程序代码
	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_Process.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);
	//MessageBox(PID);

	//创建线程对话框
	CThread* ThreadDlg = new CThread(ProcessID);
	ThreadDlg->Create(IDD_DIALOG_Thread);
	ThreadDlg->ShowWindow(SW_SHOW);

}


void CTaskManager::OnProcessModel()
{
	// TODO:  在此添加命令处理程序代码
	
	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_Process.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	//获取进程句柄
	HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
	BOOL Is32 = FALSE;
	IsWow64Process(hPro, &Is32);
	if (Is32)
	{
		//MessageBox(L"32位程序");
		//创建模块对话框
		CModule* ModuleDlg = new CModule(ProcessID);
		ModuleDlg->Create(IDD_DIALOG_Module);
		ModuleDlg->ShowWindow(SW_SHOW);
	}
	else
		MessageBox(L"请选择32位程序");

}


void CTaskManager::OnProcessTerminateprocess()
{
	// TODO:  在此添加命令处理程序代码

	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_Process.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
	TerminateProcess(hProcess, 1);
	CloseHandle(hProcess);

}

//查看桌面
BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam){

	//获取窗口名称
	LPTSTR szWindowName = new TCHAR[100];
	GetWindowText(hWnd, szWindowName, sizeof(char) * 100);

	//获取窗口类名
	LPTSTR szClassName = new TCHAR[100];
	GetClassName(hWnd, szClassName, sizeof(char) * 100);

	//通过传递过来的this指针 , 获取到m_Process
	CTaskManager* TaskManager = (CTaskManager *)lParam;
	TaskManager->m_Process.InsertItem(TaskManager->winCount, L"");

	TaskManager->m_Process.SetItemText(TaskManager->winCount, 0, szWindowName);
	TaskManager->m_Process.SetItemText(TaskManager->winCount, 1, szClassName);

	//判断窗口是否隐藏
	BOOL isVisible = IsWindowVisible(hWnd);
	if (isVisible)
	{
		TaskManager->m_Process.SetItemText(TaskManager->winCount, 2, _T("Visble"));
	}
	else
	{
		TaskManager->m_Process.SetItemText(TaskManager->winCount, 2, _T("Invisble"));
	}

	TaskManager->winCount++;
	return TRUE;
}


void CTaskManager::OnBnClickedButtonShowwin()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空原先的数据
	m_Process.DeleteAllItems();

	//删除表头
	int nColumnCount = m_Process.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_Process.DeleteColumn(0);
	}

	m_Process.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_Process.InsertColumn(0, L"窗口名称", 0, 220);
	m_Process.InsertColumn(1, L"窗口类名", 0, 120);
	m_Process.InsertColumn(2, L"是否隐藏", 0, 220);

	winCount = 0;
	//把this指针传递给了回调函数
	EnumWindows(WndEnumProc, (LPARAM)this);
}



void CTaskManager::OnBnClickedButtonMemoryControl()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建内存管理对话框
	CMemoryManager* MemoryManager= new CMemoryManager();
	MemoryManager->Create(IDD_DIALOG_Memory_Manager);
	MemoryManager->ShowWindow(SW_SHOW);
}


void CTaskManager::OnBnClickedButtonService()
{
	// TODO:  在此添加控件通知处理程序代码
	CServiceManager* ServiceManager = new CServiceManager();
	ServiceManager->Create(IDD_DIALOG_ServiceManager);
	ServiceManager->ShowWindow(SW_SHOW);
}


void CTaskManager::OnBnClickedButtonUninstall()
{
	// TODO:  在此添加控件通知处理程序代码
	CUnstall* Unstall = new CUnstall();
	Unstall->Create(IDD_DIALOG_ServiceManager);
	Unstall->ShowWindow(SW_SHOW);
}

void InjectDll(char* pDllPath, DWORD dwPid)
{
	//打开进程，获取句柄
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,  //打开权限（想要的操作）
		FALSE,               //是否可继承（本进程所创建的子进程）
		dwPid);              //进程ID
	if (hProcess == NULL)
	{
		printf("进程打开失败\n");
		return;
	}
	//在目标进程申请内存，大小是Dll路径长度
	int nLen = strlen(pDllPath) + 1;
	LPVOID lpBuf = VirtualAllocEx(
		hProcess,       //句柄
		NULL,           //申请内存的地址（0是由系统分配地址）
		nLen,           //申请字节个数
		MEM_COMMIT,     //内存状态（提交的，可以直接使用）
		PAGE_READWRITE);//内存属性（可读可写）
	if (lpBuf == NULL)
	{
		printf("内存申请失败\n");
		CloseHandle(hProcess);
		return;
	}
	//把dll路径写入到目标进程中
	DWORD dwWrite;
	WriteProcessMemory(
		hProcess, //句柄
		lpBuf,    //写入地址
		pDllPath, //写入内容
		nLen,     //写入长度
		&dwWrite);//返回写入的长度
	if (dwWrite == 0)
	{
		printf("写入失败\n");
		//释放内存
		VirtualFreeEx(hProcess, lpBuf, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return;
	}
	//创建远程线程
	HANDLE hThread = CreateRemoteThread(
		hProcess,   //目标进程句柄
		NULL,       //安全属性（0是默认）
		NULL,       //堆栈大小（0是默认）
		(LPTHREAD_START_ROUTINE)LoadLibraryA,//线程函数地址
		lpBuf,      //传给线程的参数
		NULL,       //创建标志（0是没有）
		NULL);      //线程ID
	//等待结束
	WaitForSingleObject(hThread, INFINITE);
	//释放内存
	VirtualFreeEx(hProcess, lpBuf, 0, MEM_RELEASE);
	CloseHandle(hProcess);
}

HANDLE hMapFile;
PVOID pBuf;
BOOL CreatePID(DWORD PID){
	// 1. 创建命名的文件映射
	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,
		PAGE_READWRITE,
		0,
		16,
		L"PID");

	if (NULL == hMapFile || INVALID_HANDLE_VALUE == hMapFile)
		return FALSE;

	// 2. 创建View
	pBuf = MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0, 0,
		16);
	if (NULL == pBuf)
		return FALSE;

	// 3. 将共享数据复制到文件映射中
	*((DWORD*) pBuf) = PID;

	// 5. 取消Mapping，关闭句柄
	//UnmapViewOfFile(pBuf);
	//CloseHandle(hMapFile);

	//// 1. 打开文件Mapping
	//HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, L"PID");
	//if (NULL == hMapFile)
	//	return 0;

	//// 2. 创建View
	////MessageBox(NULL, TEXT("MapViewOfFile"), NULL, 0);
	//PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 16);
	//if (NULL == pBuf)
	//	return 0;
	////g_dwProcessId = *((DWORD*)pBuf);
	//MessageBox(NULL, TEXT("1111"), NULL, 0);


	//// 5. 取消Mapping，关闭句柄
	//UnmapViewOfFile(pBuf);
	//CloseHandle(hMapFile);
}


void CTaskManager::OnProcessProtect()
{
	// TODO:  在此添加命令处理程序代码

	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_Process.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString p_name = m_Process.GetItemText(index, 0);



	//获取任务管理器PID , Taskmgr.exe
	//获取进程列表
	vector<PROCESSENTRY32> process_list = GetAllProcess();
	//WCHAR TaskManager = "Taskmgr.exe";
	char* pDllPath = "D:\\15PB\\15PBproject\\MyDll\\Debug\\MyDll.dll";
	//遍历进程列表
	for (int i = 0; i < process_list.size(); i++)
	{
		CString task(process_list[i].szExeFile);
		if (task == p_name)
		{
			DWORD PID = process_list[i].th32ProcessID;
			CreatePID(PID);
			InjectDll(pDllPath , PID);
		}
	}
}
