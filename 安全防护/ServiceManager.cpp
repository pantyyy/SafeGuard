// ServiceManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ServiceManager.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CServiceManager �Ի���

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


// CServiceManager ��Ϣ�������

void CServiceManager::TraverseService()
{
	//1.��ȡ������������
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hSCM)
	{
		MessageBox(L"�򿪷��������ʧ��!");
	}

	//2.��һ�ε� , ��ȡ��Ҫ���ڴ���Ϣ
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//3.���ݻ�ȡ���ڴ���Ϣ , ��̬�ķ����ڴ�Ĵ�С
	//��ʹ��ָ��ָ���׵�ַ
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = NULL;
	pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)malloc(dwSize);
	//��̬�ڴ��ʼ��
	memset(pEnumService , 0 , dwSize);

	//4.�ڶ���ö�� , ����̬������ڴ渳ֵ
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (LPBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//5.����������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		m_service.InsertItem(i, L"");
		//������
		CString service_name(pEnumService[i].lpDisplayName);
		m_service.SetItemText(i , 0 , service_name);

		//����ID
		CString pid;
		pid.Format(L"%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		m_service.SetItemText(i, 1, pid);

		//����
		CString description(pEnumService[i].lpDisplayName);
		m_service.SetItemText(i, 2, description);

		//��ǰ״̬
		int status = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		switch (status)
		{
		case 1:
			m_service.SetItemText(i, 3, L"��ֹͣ");
			break;
		case 2:
			m_service.SetItemText(i, 3, L"��������");
			break;
		case 3:
			m_service.SetItemText(i, 3, L"����ֹͣ");
			break;
		case 4:
			m_service.SetItemText(i, 3, L"��������");
			break;
		case 5:
			m_service.SetItemText(i, 3, L"��������");
			break;
		}

	}

}

BOOL CServiceManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//���öԻ�����
	CString DlgTitle("�������");
	SetWindowText(DlgTitle.GetBuffer());

	m_service.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_service.InsertColumn(0, L"������", 0, 200);
	m_service.InsertColumn(1, L"PID", 0, 50);
	m_service.InsertColumn(2, L"����", 0, 370);
	m_service.InsertColumn(3, L"��ǰ״̬", 0, 100);


	TraverseService();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CServiceManager::OnServiceClose()
{
	// TODO:  �ڴ���������������
	//��ȡѡ�еķ�����
	POSITION ps;
	int index;
	ps = m_service.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_service.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString service_name = m_service.GetItemText(index, 0);

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if ((long)hSCM == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"����ķ��ʱ��ܾ�");
	}
	else if ((long)hSCM == ERROR_DATABASE_DOES_NOT_EXIST)
	{
		MessageBox(L"ָ�������ݿⲻ����");
	}

	//��ȡ������
	SC_HANDLE hService = OpenService(hSCM, service_name, SERVICE_ALL_ACCESS);
	if ((long)hService == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"�����Ȩ���ʸ÷���");
	}

	SERVICE_STATUS status;
	ControlService(hService, SERVICE_CONTROL_STOP, &status);
	CloseServiceHandle(hSCM);

	m_service.DeleteAllItems();
	TraverseService();
}


void CServiceManager::OnServiceStart()
{
	// TODO:  �ڴ���������������
	//��ȡѡ�еķ�����
	POSITION ps;
	int index;
	ps = m_service.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_service.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString service_name = m_service.GetItemText(index, 0);

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL , NULL , SC_MANAGER_ALL_ACCESS);

	if ((long)hSCM == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"����ķ��ʱ��ܾ�");
	}
	else if ((long)hSCM == ERROR_DATABASE_DOES_NOT_EXIST)
	{
		MessageBox(L"ָ�������ݿⲻ����");
	}

	//��ȡ������
	SC_HANDLE hService = OpenService(hSCM , service_name , SERVICE_ALL_ACCESS);
	if ((long)hService == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"�����Ȩ���ʸ÷���");
	}

	//��������
	StartService(hService , NULL , NULL);

	CloseServiceHandle(hSCM);
	m_service.DeleteAllItems();
	TraverseService();


}


void CServiceManager::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//�����˵�
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Service)); //���ز˵�
	HMENU hSub = GetSubMenu(hMenu, 0); //��ȡ�Ӳ˵����
	CPoint pt;
	GetCursorPos(&pt); //��ȡ����
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

}
