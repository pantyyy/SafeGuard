// MemoryManager.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "MemoryManager.h"
#include "afxdialogex.h"
#include "Psapi.h"

// CMemoryManager 对话框

IMPLEMENT_DYNAMIC(CMemoryManager, CDialogEx)

CMemoryManager::CMemoryManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMemoryManager::IDD, pParent)
	, m_CPU(_T(""))
	, m_Memory(_T(""))
{

}

CMemoryManager::~CMemoryManager()
{
}

void CMemoryManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CPU);
	DDX_Text(pDX, IDC_EDIT4, m_Memory);
}


BEGIN_MESSAGE_MAP(CMemoryManager, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MemoryOptimize, &CMemoryManager::OnBnClickedButtonMemoryoptimize)
END_MESSAGE_MAP()


// CMemoryManager 消息处理程序




__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
	return   (b - a);
}


//CPu使用率计算
int GetCPU()
{
	//空闲时间 , 内核时间 , 用户时间
	FILETIME idleTime, kernelTime, userTime;

	//第一次获取处理器时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//创建内核对象并等待1000毫秒
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);

	//第二次获取处理器时间
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

	int idle = CompareFileTime(preidleTime, idleTime);
	int kernel = CompareFileTime(prekernelTime, kernelTime);
	int user = CompareFileTime(preuserTime, userTime);

	//(总的时间 - 空闲时间) / 总的时间
	int cpu = (100 - idle) * 100 / (kernel + user);

	return cpu;
}

	
int GetMemory()
{
	//获取当前内存状态信息
	MEMORYSTATUSEX stcMemStatusEx = {0};
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);

	//已经使用的内存 = 总的物理内存 - 可用的物理内存
	DWORDLONG UsedMemory = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	//内存占用率 = 已经使用的内存 / 总的物理内存
	DWORD MemoryRate = ((double)UsedMemory / (double)stcMemStatusEx.ullTotalPhys) * 100;
	return MemoryRate;
}


void CMemoryManager::OnBnClickedButtonMemoryoptimize()
{
	// TODO:  在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMemStatusEX = { 0 };
	stcMemStatusEX.dwLength = sizeof(stcMemStatusEX);
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG preUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;
	//内存清理
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);

	}

	//获取内存使用率
	DWORD Memory = GetMemory();
	CString Memory_Str;
	Memory_Str.Format(L"%d", Memory);
	m_Memory = Memory_Str;


	DWORD CPU = GetCPU();
	CString CPU_Str;
	CPU_Str.Format(L"%d", CPU);
	m_CPU = CPU_Str;

	UpdateData(FALSE);

}

BOOL CMemoryManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框名
	CString DlgTitle("内存管理");
	SetWindowText(DlgTitle.GetBuffer());

	//获取CPU使用率
	DWORD CPU = GetCPU();
	CString CPU_Str;
	CPU_Str.Format(L"%d", CPU);
	m_CPU = CPU_Str;

	//获取内存使用率
	DWORD Memory = GetMemory();
	CString Memory_Str;
	Memory_Str.Format(L"%d" , Memory);
	m_Memory = Memory_Str;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
