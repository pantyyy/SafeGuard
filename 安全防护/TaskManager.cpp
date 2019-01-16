// TaskManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "TaskManager.h"
#include "afxdialogex.h"
#include "Thread.h"
#include "Module.h"
#include "MemoryManager.h"
#include "ServiceManager.h"
#include<windows.h>
#include<TlHelp32.h>
#include<vector>
using std::vector;


//��ȡ���н��̶���
vector<PROCESSENTRY32> GetAllProcess()
{
	vector<PROCESSENTRY32> process_list;
	//1.�������̿���
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	//2.���̽ṹ�� , ���������������ÿһ������
	PROCESSENTRY32 process_info = { sizeof(PROCESSENTRY32) };

	//3.���Ի�ȡ�����еĵ�һ������ , �ж��Ƿ�ɹ�
	if (Process32First(snapshot, &process_info))
	{
		//�����ȡ���Ľ�����Ϣ
		do
		{
			process_list.push_back(process_info);

		} while (Process32Next(snapshot, &process_info));//��ȡ��һ��������Ϣ
	}

	return process_list;
}



// CTaskManager �Ի���

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
END_MESSAGE_MAP()


// CTaskManager ��Ϣ�������


BOOL CTaskManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���öԻ�����
	CString DlgTitle("���������");
	SetWindowText(DlgTitle.GetBuffer());

	//���ñ�ͷ
	m_Process.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Process.InsertColumn(0, L"����", 0, 200);
	m_Process.InsertColumn(1, L"����ID", 0, 80);
	m_Process.InsertColumn(2, L"������ID", 0, 80);
	m_Process.InsertColumn(3, L"�߳���", 0, 80);
	m_Process.InsertColumn(4, L"λ��", 0, 80);
	m_Process.InsertColumn(5, L"·��", 0, 500);

	//��ȡ�����б�
	vector<PROCESSENTRY32> process_list = GetAllProcess();

	//���������б�
	for (int i = 0; i < process_list.size(); i++)
	{
		int j = 0;
		//������
		m_Process.InsertItem(i, process_list[i].szExeFile);

		//����ID
		CString c_id;
		c_id.Format(TEXT("%d"), process_list[i].th32ProcessID);
		m_Process.SetItemText(i, ++j, c_id);

		//������ID
		CString p_id;
		p_id.Format(TEXT("%d"), process_list[i].th32ParentProcessID);
		m_Process.SetItemText(i, ++j, p_id);

		//�߳���
		CString count_thread;
		count_thread.Format(TEXT("%d"), process_list[i].cntThreads);
		m_Process.SetItemText(i, ++j, count_thread);

		//��������·��
		//1.��ȡ���̾��
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_list[i].th32ProcessID);
		//2.�������鱣��·��
		DWORD length = MAX_PATH;
		WCHAR path[MAX_PATH] = { 0 };
		//3.��������·��
		QueryFullProcessImageName(process, 0, path, &length);


		BOOL Is32 = FALSE;
		//64 - bit process on 64 - bit Windows : FALSE
		//32 - bit process on 64 - bit Windows : TRUE
		IsWow64Process(process, &Is32);
		if (Is32)
			m_Process.SetItemText(i, ++j, L"32λ");
		else
			m_Process.SetItemText(i, ++j, L"64λ");

		m_Process.SetItemText(i, ++j, path);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CTaskManager::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�����˵�
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Process)); //���ز˵�
	HMENU hSub = GetSubMenu(hMenu, 0); //��ȡ�Ӳ˵����
	CPoint pt;
	GetCursorPos(&pt); //��ȡ����
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);


	*pResult = 0;
}


void CTaskManager::OnProcessThread()
{
	// TODO:  �ڴ���������������
	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_Process.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);
	//MessageBox(PID);

	//�����̶߳Ի���
	CThread* ThreadDlg = new CThread(ProcessID);
	ThreadDlg->Create(IDD_DIALOG_Thread);
	ThreadDlg->ShowWindow(SW_SHOW);

}


void CTaskManager::OnProcessModel()
{
	// TODO:  �ڴ���������������
	
	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_Process.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	//��ȡ���̾��
	HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
	BOOL Is32 = FALSE;
	IsWow64Process(hPro, &Is32);
	if (Is32)
	{
		//MessageBox(L"32λ����");
		//����ģ��Ի���
		CModule* ModuleDlg = new CModule(ProcessID);
		ModuleDlg->Create(IDD_DIALOG_Module);
		ModuleDlg->ShowWindow(SW_SHOW);
	}
	else
		MessageBox(L"��ѡ��32λ����");

}


void CTaskManager::OnProcessTerminateprocess()
{
	// TODO:  �ڴ���������������

	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_Process.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_Process.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString PID = m_Process.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
	TerminateProcess(hProcess, 1);
	CloseHandle(hProcess);

}

//�鿴����
BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam){

	//��ȡ��������
	LPTSTR szWindowName = new TCHAR[100];
	GetWindowText(hWnd, szWindowName, sizeof(char) * 100);

	//��ȡ��������
	LPTSTR szClassName = new TCHAR[100];
	GetClassName(hWnd, szClassName, sizeof(char) * 100);

	//ͨ�����ݹ�����thisָ�� , ��ȡ��m_Process
	CTaskManager* TaskManager = (CTaskManager *)lParam;
	TaskManager->m_Process.InsertItem(TaskManager->winCount, L"");

	TaskManager->m_Process.SetItemText(TaskManager->winCount, 0, szWindowName);
	TaskManager->m_Process.SetItemText(TaskManager->winCount, 1, szClassName);

	//�жϴ����Ƿ�����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ԭ�ȵ�����
	m_Process.DeleteAllItems();

	//ɾ����ͷ
	int nColumnCount = m_Process.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_Process.DeleteColumn(0);
	}

	m_Process.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_Process.InsertColumn(0, L"��������", 0, 220);
	m_Process.InsertColumn(1, L"��������", 0, 120);
	m_Process.InsertColumn(2, L"�Ƿ�����", 0, 220);

	winCount = 0;
	//��thisָ�봫�ݸ��˻ص�����
	EnumWindows(WndEnumProc, (LPARAM)this);
}



void CTaskManager::OnBnClickedButtonMemoryControl()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�����ڴ����Ի���
	CMemoryManager* MemoryManager= new CMemoryManager();
	MemoryManager->Create(IDD_DIALOG_Memory_Manager);
	MemoryManager->ShowWindow(SW_SHOW);
}


void CTaskManager::OnBnClickedButtonService()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CServiceManager* ServiceManager = new CServiceManager();
	ServiceManager->Create(IDD_DIALOG_ServiceManager);
	ServiceManager->ShowWindow(SW_SHOW);
}
