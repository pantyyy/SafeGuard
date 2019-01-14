// MemoryManager.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "MemoryManager.h"
#include "afxdialogex.h"


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


void CMemoryManager::OnBnClickedButtonMemoryoptimize()
{
	// TODO:  在此添加控件通知处理程序代码

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

	//转换上面得到的时间
	double 空闲时间1 = (double)(idleTime.dwHighDateTime*4.294967296E9) + (double)idleTime.dwLowDateTime;
	double 空闲时间2 = (double)(preidleTime.dwHighDateTime*4.294967296E9) + (double)preidleTime.dwLowDateTime;
	double 内核时间1 = (double)(kernelTime.dwHighDateTime*4.294967296E9) + (double)kernelTime.dwLowDateTime;
	double 内核时间2 = (double)(prekernelTime.dwHighDateTime*4.294967296E9) + (double)prekernelTime.dwLowDateTime;
	double 用户时间1 = (double)(userTime.dwHighDateTime*4.294967296E9) + (double)userTime.dwLowDateTime;
	double 用户时间2 = (double)(preuserTime.dwHighDateTime*4.294967296E9) + (double)preuserTime.dwLowDateTime;

	int 使用率 = (int)(100.0 - (空闲时间2 - 空闲时间1) / (内核时间2 - 内核时间1 + 用户时间2 - 用户时间1)*100.0);
	return 使用率;

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
