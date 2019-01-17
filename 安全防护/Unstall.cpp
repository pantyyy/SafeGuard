// Unstall.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Unstall.h"
#include "afxdialogex.h"
#include<vector>
using std::vector;

// CUnstall 对话框
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


// CUnstall 消息处理程序


BOOL CUnstall::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString WinTitle("软件卸载");
	SetWindowText(WinTitle);

	m_uninstall_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_uninstall_list.InsertColumn(0, L"软件名称", 0, 100);
	m_uninstall_list.InsertColumn(1, L"版本", 0, 100);
	m_uninstall_list.InsertColumn(2, L"厂商", 0, 100);
	m_uninstall_list.InsertColumn(3, L"安装路径", 0, 100);
	m_uninstall_list.InsertColumn(4, L"卸载路径", 0, 80);

	TraverseUninstallSoftware();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CUnstall::TraverseUninstallSoftware()
{
	//自定义结构体
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];	//软件名称
		WCHAR szSoftVer[50];	//版本
		//WCHAR szSoftDate[20];	//安装日期
		//DWORD szSoftSize = 0;	//大小
		WCHAR strSoftInsPath[MAX_PATH];	//安装路径
		WCHAR strSoftUniPath[MAX_PATH];
		WCHAR strSoftVenRel[50];	//厂商
	}SOFTINFO, *PSOFTINFO;

	vector<SOFTINFO> unstall_list;		//卸载列表数组
	SOFTINFO m_SofteInfo;
	HKEY RootKey = HKEY_LOCAL_MACHINE; //主键
	LPCTSTR lpSubKey = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;	//接收将要打开的键的句柄
	//打开一个已经存在的注册表键
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





	//循环遍历Uninstall目录下的子健
	DWORD dwIndex = 0;
	for (i = 0; i<cSubKeys; i++)
	{
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};	//注册表项名称
		LONG lReturn = RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL);

		//通过得到的子健名称重新组合成新的子健路径
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);

		//打开新的子健 , 获取其句柄
		HKEY hkValueKey = 0;
		RegOpenKeyEx(RootKey , strMidReg , 0 , KEY_QUERY_VALUE , &hkValueKey);

		//获取键值
		DWORD dwNameLen = 255;
		DWORD type = REG_SZ;
		DWORD type2 = REG_DWORD;
		//获取软件名称
		//DWORD dwType
		//WCHAR szSoftName[50];	//软件名称(DisplayName)
		//WCHAR szSoftVer[50];	//版本(DisplayVersion)
		//WCHAR szSoftDate[20];	//安装日期()
		//WCHAR szSoftSize[MAX_PATH];	//大小(EstimatedSize)
		//WCHAR strSoftInsPath[MAX_PATH];	//安装路径(InstallLocation)
		//WCHAR strSoftVenRel[50];	//厂商(Publisher)
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

	//数据写入list控件中
	for (int i = 0; i < unstall_list.size(); i++)
	{
		//名称
		CString softname(unstall_list[i].szSoftName);
		m_uninstall_list.InsertItem(i, L"");
		m_uninstall_list.SetItemText(i, 0, softname);


		//版本
		CString version(unstall_list[i].szSoftVer);
		m_uninstall_list.SetItemText(i, 1, version);

		//厂商
		CString publisher(unstall_list[i].strSoftVenRel);
		m_uninstall_list.SetItemText(i, 2, publisher);

		//安装路径
		CString inspath(unstall_list[i].strSoftInsPath);
		m_uninstall_list.SetItemText(i, 3, inspath);

		//卸载路径
		CString uninspath(unstall_list[i].strSoftUniPath);
		m_uninstall_list.SetItemText(i, 4, uninspath);
	}



}


void CUnstall::OnUninstallSoftware()
{
	// TODO:  在此添加命令处理程序代码
	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_uninstall_list.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_uninstall_list.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString usinspath = m_uninstall_list.GetItemText(index, 4);

	char* file_path = new char[520];
	WideCharToMultiByte(CP_ACP, 0, usinspath, -1, file_path, 520, NULL, NULL);
	WinExec(file_path, SW_SHOW);
}


void CUnstall::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	//弹出菜单
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Uninstall)); //加载菜单
	HMENU hSub = GetSubMenu(hMenu, 0); //获取子菜单句柄
	CPoint pt;
	GetCursorPos(&pt); //获取坐标
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

	*pResult = 0;
}
