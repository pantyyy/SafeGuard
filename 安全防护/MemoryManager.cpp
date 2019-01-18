// MemoryManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "MemoryManager.h"
#include "afxdialogex.h"
#include "Psapi.h"

// CMemoryManager �Ի���

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


// CMemoryManager ��Ϣ�������




__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
	return   (b - a);
}


//CPuʹ���ʼ���
int GetCPU()
{
	//����ʱ�� , �ں�ʱ�� , �û�ʱ��
	FILETIME idleTime, kernelTime, userTime;

	//��һ�λ�ȡ������ʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//�����ں˶��󲢵ȴ�1000����
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);

	//�ڶ��λ�ȡ������ʱ��
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

	int idle = CompareFileTime(preidleTime, idleTime);
	int kernel = CompareFileTime(prekernelTime, kernelTime);
	int user = CompareFileTime(preuserTime, userTime);

	//(�ܵ�ʱ�� - ����ʱ��) / �ܵ�ʱ��
	int cpu = (100 - idle) * 100 / (kernel + user);

	return cpu;
}

	
int GetMemory()
{
	//��ȡ��ǰ�ڴ�״̬��Ϣ
	MEMORYSTATUSEX stcMemStatusEx = {0};
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);

	//�Ѿ�ʹ�õ��ڴ� = �ܵ������ڴ� - ���õ������ڴ�
	DWORDLONG UsedMemory = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	//�ڴ�ռ���� = �Ѿ�ʹ�õ��ڴ� / �ܵ������ڴ�
	DWORD MemoryRate = ((double)UsedMemory / (double)stcMemStatusEx.ullTotalPhys) * 100;
	return MemoryRate;
}


void CMemoryManager::OnBnClickedButtonMemoryoptimize()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	MEMORYSTATUSEX stcMemStatusEX = { 0 };
	stcMemStatusEX.dwLength = sizeof(stcMemStatusEX);
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG preUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;
	//�ڴ�����
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);

	}

	//��ȡ�ڴ�ʹ����
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���öԻ�����
	CString DlgTitle("�ڴ����");
	SetWindowText(DlgTitle.GetBuffer());

	//��ȡCPUʹ����
	DWORD CPU = GetCPU();
	CString CPU_Str;
	CPU_Str.Format(L"%d", CPU);
	m_CPU = CPU_Str;

	//��ȡ�ڴ�ʹ����
	DWORD Memory = GetMemory();
	CString Memory_Str;
	Memory_Str.Format(L"%d" , Memory);
	m_Memory = Memory_Str;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
