// MemoryManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "MemoryManager.h"
#include "afxdialogex.h"


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


void CMemoryManager::OnBnClickedButtonMemoryoptimize()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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

	//ת������õ���ʱ��
	double ����ʱ��1 = (double)(idleTime.dwHighDateTime*4.294967296E9) + (double)idleTime.dwLowDateTime;
	double ����ʱ��2 = (double)(preidleTime.dwHighDateTime*4.294967296E9) + (double)preidleTime.dwLowDateTime;
	double �ں�ʱ��1 = (double)(kernelTime.dwHighDateTime*4.294967296E9) + (double)kernelTime.dwLowDateTime;
	double �ں�ʱ��2 = (double)(prekernelTime.dwHighDateTime*4.294967296E9) + (double)prekernelTime.dwLowDateTime;
	double �û�ʱ��1 = (double)(userTime.dwHighDateTime*4.294967296E9) + (double)userTime.dwLowDateTime;
	double �û�ʱ��2 = (double)(preuserTime.dwHighDateTime*4.294967296E9) + (double)preuserTime.dwLowDateTime;

	int ʹ���� = (int)(100.0 - (����ʱ��2 - ����ʱ��1) / (�ں�ʱ��2 - �ں�ʱ��1 + �û�ʱ��2 - �û�ʱ��1)*100.0);
	return ʹ����;

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
