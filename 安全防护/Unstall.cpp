// Unstall.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "Unstall.h"
#include "afxdialogex.h"
#include<vector>
using std::vector;

// CUnstall �Ի���
#define MAX_KEY_LENGTH 255  
#define MAX_VALUE_NAME 16383  


IMPLEMENT_DYNAMIC(CUnstall, CDialogEx)

CUnstall::CUnstall(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnstall::IDD, pParent)
{

}

CUnstall::~CUnstall()
{
}

void CUnstall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_uninstall_list);
}


BEGIN_MESSAGE_MAP(CUnstall, CDialogEx)
	ON_COMMAND(ID_UNINSTALL_Software, &CUnstall::OnUninstallSoftware)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CUnstall::OnNMRClickList1)
END_MESSAGE_MAP()


// CUnstall ��Ϣ�������


BOOL CUnstall::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString WinTitle("���ж��");
	SetWindowText(WinTitle);

	m_uninstall_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_uninstall_list.InsertColumn(0, L"�������", 0, 100);
	m_uninstall_list.InsertColumn(1, L"�汾", 0, 100);
	m_uninstall_list.InsertColumn(2, L"����", 0, 100);
	m_uninstall_list.InsertColumn(3, L"��װ·��", 0, 100);
	m_uninstall_list.InsertColumn(4, L"ж��·��", 0, 80);

	TraverseUninstallSoftware();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CUnstall::TraverseUninstallSoftware()
{
	//�Զ���ṹ��
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];	//�������
		WCHAR szSoftVer[50];	//�汾
		//WCHAR szSoftDate[20];	//��װ����
		//DWORD szSoftSize = 0;	//��С
		WCHAR strSoftInsPath[MAX_PATH];	//��װ·��
		WCHAR strSoftUniPath[MAX_PATH];
		WCHAR strSoftVenRel[50];	//����
	}SOFTINFO, *PSOFTINFO;

	vector<SOFTINFO> unstall_list;		//ж���б�����
	SOFTINFO m_SofteInfo;
	HKEY RootKey = HKEY_LOCAL_MACHINE; //����
	LPCTSTR lpSubKey = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;	//���ս�Ҫ�򿪵ļ��ľ��
	//��һ���Ѿ����ڵ�ע����
	LONG lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hkResult);



	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name  
	DWORD    cbName;                   // size of name string   
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name   
	DWORD    cchClassName = MAX_PATH;  // size of class string   
	DWORD    cSubKeys = 0;               // number of subkeys   
	DWORD    cbMaxSubKey;              // longest subkey size   
	DWORD    cchMaxClass;              // longest class string   
	DWORD    cValues;              // number of values for key   
	DWORD    cchMaxValue;          // longest value name   
	DWORD    cbMaxValueData;       // longest value data   
	DWORD    cbSecurityDescriptor; // size of security descriptor   
	FILETIME ftLastWriteTime;      // last write time   

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;



	retCode = RegQueryInfoKey(
		hkResult,                    // key handle   
		achClass,                // buffer for class name   
		&cchClassName,           // size of class string   
		NULL,                    // reserved   
		&cSubKeys,               // number of subkeys   
		&cbMaxSubKey,            // longest subkey size   
		&cchMaxClass,            // longest class string   
		&cValues,                // number of values for this key   
		&cchMaxValue,            // longest value name   
		&cbMaxValueData,         // longest value data   
		&cbSecurityDescriptor,   // security descriptor   
		&ftLastWriteTime);       // last write time   





	//ѭ������UninstallĿ¼�µ��ӽ�
	DWORD dwIndex = 0;
	for (i = 0; i<cSubKeys; i++)
	{
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};	//ע���������
		LONG lReturn = RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL);

		//ͨ���õ����ӽ�����������ϳ��µ��ӽ�·��
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);

		//���µ��ӽ� , ��ȡ����
		HKEY hkValueKey = 0;
		RegOpenKeyEx(RootKey , strMidReg , 0 , KEY_QUERY_VALUE , &hkValueKey);

		//��ȡ��ֵ
		DWORD dwNameLen = 255;
		DWORD type = REG_SZ;
		DWORD type2 = REG_DWORD;
		//��ȡ�������
		//DWORD dwType
		//WCHAR szSoftName[50];	//�������(DisplayName)
		//WCHAR szSoftVer[50];	//�汾(DisplayVersion)
		//WCHAR szSoftDate[20];	//��װ����()
		//WCHAR szSoftSize[MAX_PATH];	//��С(EstimatedSize)
		//WCHAR strSoftInsPath[MAX_PATH];	//��װ·��(InstallLocation)
		//WCHAR strSoftVenRel[50];	//����(Publisher)
		//RegQueryValueEx(hkValueKey, TEXT("EstimatedSize"), 0, &type2, (LPBYTE)m_SofteInfo.szSoftSize, &dwNameLen);
		RegQueryValueEx(hkValueKey, TEXT("InstallLocation"), 0, &type, (LPBYTE)m_SofteInfo.strSoftInsPath, &dwNameLen);
		RegQueryValueEx(hkValueKey, TEXT("Publisher"), 0, &type, (LPBYTE)m_SofteInfo.strSoftVenRel, &dwNameLen);

		RegQueryValueEx(hkValueKey, TEXT("DisplayVersion"), 0, &type, (LPBYTE)m_SofteInfo.szSoftVer, &dwNameLen);
		RegQueryValueEx(hkValueKey, TEXT("UninstallString"), 0, &type, (LPBYTE)m_SofteInfo.strSoftUniPath, &dwNameLen);
		RegQueryValueEx(hkValueKey, TEXT("DisplayName"), 0, &type, (LPBYTE)m_SofteInfo.szSoftName, &dwNameLen);
		
		unstall_list.push_back(m_SofteInfo);
		dwNameLen = 255;
		dwIndex++;
	}

	//����д��list�ؼ���
	for (int i = 0; i < unstall_list.size(); i++)
	{
		//����
		CString softname(unstall_list[i].szSoftName);
		m_uninstall_list.InsertItem(i, L"");
		m_uninstall_list.SetItemText(i, 0, softname);


		//�汾
		CString version(unstall_list[i].szSoftVer);
		m_uninstall_list.SetItemText(i, 1, version);

		//����
		CString publisher(unstall_list[i].strSoftVenRel);
		m_uninstall_list.SetItemText(i, 2, publisher);

		//��װ·��
		CString inspath(unstall_list[i].strSoftInsPath);
		m_uninstall_list.SetItemText(i, 3, inspath);

		//ж��·��
		CString uninspath(unstall_list[i].strSoftUniPath);
		m_uninstall_list.SetItemText(i, 4, uninspath);
	}



}


void CUnstall::OnUninstallSoftware()
{
	// TODO:  �ڴ���������������
	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_uninstall_list.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_uninstall_list.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString usinspath = m_uninstall_list.GetItemText(index, 4);

	char* file_path = new char[520];
	WideCharToMultiByte(CP_ACP, 0, usinspath, -1, file_path, 520, NULL, NULL);
	WinExec(file_path, SW_SHOW);
}


void CUnstall::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�����˵�
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Uninstall)); //���ز˵�
	HMENU hSub = GetSubMenu(hMenu, 0); //��ȡ�Ӳ˵����
	CPoint pt;
	GetCursorPos(&pt); //��ȡ����
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

	*pResult = 0;
}
