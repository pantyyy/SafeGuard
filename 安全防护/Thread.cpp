// Thread.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Thread.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <TlHelp32.h>

// CThread 对话框

IMPLEMENT_DYNAMIC(CThread, CDialogEx)

CThread::CThread(DWORD pid, CWnd* pParent /*=NULL*/)
	: CDialogEx(CThread::IDD, pParent)
{
	nPid = pid;
}


CThread::~CThread()
{
}

void CThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Thread);
}


BEGIN_MESSAGE_MAP(CThread, CDialogEx)
END_MESSAGE_MAP()


// CThread 消息处理程序


BOOL CThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框名
	CString DlgTitle("线程信息");
	SetWindowText(DlgTitle.GetBuffer());

	m_Thread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Thread.InsertColumn(0, L"线程ID", 0, 80);
	m_Thread.InsertColumn(1, L"进程ID", 0, 80);
	m_Thread.InsertColumn(2, L"默认的优先级", 0, 150);

	// 2. 创建一个线程快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);

	// 3. 创建一个结构体用于保存线程的信息
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	int nCount = 0;
	// 4. 尝试从快照中获取到第一个线程
	if (Thread32First(Snapshot, &ThreadInfo))
	{
		// 5. 对遍历到的数据进行输出
		do {
			// 如果遍历到的线程父进程为PID就进行输出
			if (ThreadInfo.th32OwnerProcessID == nPid)
			{
				m_Thread.InsertItem(nCount, L"");

				// 线程的ID
				CString t_id;
				t_id.Format(TEXT("%d") , ThreadInfo.th32ThreadID);
				m_Thread.SetItemText(nCount, 0 , t_id);

				// 进程的ID
				CString p_id;
				p_id.Format(TEXT("%d") , nPid);
				m_Thread.SetItemText(nCount, 1, p_id);

				// 默认的优先级，优先级越高，调度的越多
				CString priority;
				priority.Format(TEXT("%d"), ThreadInfo.tpBasePri);
				m_Thread.SetItemText(nCount, 2, priority);

				nCount++;
			}
			//6. 尝试获取下一个线程的信息
		} while (Thread32Next(Snapshot, &ThreadInfo));
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
