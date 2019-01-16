// KillVirus.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "KillVirus.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "Md5.h"
#include "WhiteListKill.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

#define WM_MYSOCKET WM_USER+100

// CKillVirus 对话框

IMPLEMENT_DYNAMIC(CKillVirus, CDialogEx)

CKillVirus::CKillVirus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKillVirus::IDD, pParent)
{

}

CKillVirus::~CKillVirus()
{
}

void CKillVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_virus_list);
	DDX_Text(pDX, IDC_EDIT1, m_path);
}


BEGIN_MESSAGE_MAP(CKillVirus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MD5, &CKillVirus::OnBnClickedButtonMd5)
	ON_BN_CLICKED(IDC_BUTTON_Kill_By_Path, &CKillVirus::OnBnClickedButtonKillByPath)
	ON_BN_CLICKED(IDC_BUTTON_White_List, &CKillVirus::OnBnClickedButtonWhiteList)
	ON_BN_CLICKED(IDC_BUTTON_Cloud_Kill, &CKillVirus::OnBnClickedButtonCloudKill)
	ON_BN_CLICKED(IDC_BUTTON_Path, &CKillVirus::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_Scan, &CKillVirus::OnBnClickedButtonScan)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CKillVirus::OnNMRClickList1)
	ON_COMMAND(ID_VIRUS__AddToLocal, &CKillVirus::OnVirus)
	ON_COMMAND(ID_VIRUS_UploadVirus, &CKillVirus::OnVirusUploadvirus)
	ON_BN_CLICKED(IDC_BUTTON_Clean_Virus, &CKillVirus::OnBnClickedButtonCleanVirus)
	ON_MESSAGE(WM_MYSOCKET, &CKillVirus::OnMysocket)
END_MESSAGE_MAP()


// CKillVirus 消息处理程序


BOOL CKillVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString DlgTitle("病毒查杀");
	SetWindowText(DlgTitle.GetBuffer());

	m_virus_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_virus_list.InsertColumn(0, L"病毒名字", 0, 150);
	m_virus_list.InsertColumn(1, L"病毒路径", 0, 300);
	m_virus_list.InsertColumn(2, L"MD5", 0, 250);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CKillVirus::OnBnClickedButtonMd5()
{
	// TODO:  在此添加控件通知处理程序代码

	//1.获取指定路径所有的文件
	m_virus_list.DeleteAllItems();
	std::vector<FileInfo> fileList;
	CTool::GetAllFile(m_path.GetBuffer(), fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"没有找到符合要求的文件!");
		return;
	}

	//2.读取本地本地病毒库数据
	vector<char*> local_md5;
	char line[512] = { 0 };
	std::ifstream fin("Virus_Database.txt", std::ios::in);
	while (fin.getline(line , sizeof(line))){
		char* temp = new char[520];
		memcpy(temp , line , sizeof(line));
		local_md5.push_back(temp);
	}

	int nCount = 0;
	for (int i = 0; i < fileList.size(); i++)
	{
		char* file_md5 = new char[520];
		WideCharToMultiByte(CP_ACP, 0, fileList[i].cFilePath, -1, file_md5, 520, NULL, NULL);
		char * szBuff = md5FileValue(file_md5);

		for (int j = 0; j < local_md5.size(); j++){
			////3.根据MD5值判断是否是病毒
			if (strcmp(szBuff , local_md5[j]) == 0)
			{
				m_virus_list.InsertItem(nCount, L"");
				//病毒名
				CString name(fileList[i].cFileName);
				m_virus_list.SetItemText(nCount, 0, name);

				//病毒路劲
				CString path(fileList[i].cFilePath);
				m_virus_list.SetItemText(nCount, 1, path);

				//MD5值
				CString virus_md5;
				virus_md5.Format(L"%S", local_md5[j]);
				m_virus_list.SetItemText(nCount, 2, virus_md5);

				nCount++;
			}
		}

	}


	
}


void CKillVirus::OnBnClickedButtonKillByPath()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CKillVirus::OnBnClickedButtonWhiteList()
{
	// TODO:  在此添加控件通知处理程序代码
	CWhiteListKill* WhiteListKill = new CWhiteListKill();
	WhiteListKill->Create(IDD_DIALOG_White_List_Kill);
	WhiteListKill->ShowWindow(SW_SHOW);

}


void CKillVirus::OnBnClickedButtonCloudKill()
{
	// TODO:  在此添加控件通知处理程序代码

	//连接服务器
	if (!CTool::m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败！", L"Error!", MB_OK || MB_ICONWARNING);
		return;
	}

	//服务器连接成功

	//向服务器发送获取病毒MD5值的信息
	CStringA str("获取病毒MD5");
	CTool::m_client.SendMsg(str.GetBuffer() , str.GetLength() , GetVirusMD5);

	//接收所有服务器发送过来的数据
	WSAAsyncSelect(CTool::m_client.m_sClient, m_hWnd, WM_MYSOCKET, FD_READ | FD_CLOSE);

}


void CKillVirus::OnBnClickedButtonPath()
{
	// TODO:  在此添加控件通知处理程序代码
	CFolderPickerDialog dlg;
	dlg.DoModal();

	CString path = dlg.GetFolderPath();
	m_path = path;
	UpdateData(FALSE);
}


void CKillVirus::OnBnClickedButtonScan()
{
	// TODO:  在此添加控件通知处理程序代码
	m_virus_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//定义一个存放结果文件名称的链表
	CTool::GetAllFile(m_path.GetBuffer(), fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"没有找到符合要求的文件!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_virus_list.InsertItem(i, L"");
		//文件名
		CString name(fileList[i].cFileName);
		m_virus_list.SetItemText(i, 0, name);

		//文件路径
		CString path(fileList[i].cFilePath);
		m_virus_list.SetItemText(i, 1, path);

		//文件MD5值
		//宽字节转多字节
		char* file_path_byte = new char[520];
		WideCharToMultiByte(CP_ACP, 0, fileList[i].cFilePath, -1, file_path_byte, 520, NULL, NULL);

		char* file_md5_str = md5FileValue(file_path_byte);

		//char* 转 CString
		CString file_md5;
		file_md5.Format(L"%S", file_md5_str);
		m_virus_list.SetItemText(i, 2, file_md5);

	}
}


void CKillVirus::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	//弹出菜单
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Virus)); //加载菜单
	HMENU hSub = GetSubMenu(hMenu, 0); //获取子菜单句柄
	CPoint pt;
	GetCursorPos(&pt); //获取坐标
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);


	*pResult = 0;
}

//添加病毒到本地
void CKillVirus::OnVirus()
{
	// TODO:  在此添加命令处理程序代码
	//MessageBox(L"添加病毒");
	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_virus_list.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_virus_list.GetNextSelectedItem(ps);
	
	//获取选中行中指定列的数据 , 0表示多少列
	CString virus_md5 = m_virus_list.GetItemText(index, 2);
	//宽字节转多字节
	char * virus_arr = new char[520];
	WideCharToMultiByte(CP_ACP, 0, virus_md5, -1, virus_arr, 520, NULL, NULL);

	//向文件中写入数据
	ofstream ofresult("Virus_Database.txt", ios::app);
	ofresult << virus_arr << endl;
	MessageBox(L"添加病毒成功!");
}


void CKillVirus::OnVirusUploadvirus()
{
	// TODO:  在此添加命令处理程序代码
	HANDLE hFile;//定义一个句柄。   
	hFile = CreateFile(L"Virus_Database.txt",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);//使用CreatFile这个API函数打开文件   
	DWORD Written;
	WriteFile(hFile, "Welcome to VCFans!", strlen("Welcome to VCFans!"), &Written, NULL);//写入文件   
	CloseHandle(hFile);//关闭句柄   
}


void CKillVirus::OnBnClickedButtonCleanVirus()
{
	// TODO:  在此添加控件通知处理程序代码



}


//接收服务器发送过来的消息
afx_msg LRESULT CKillVirus::OnMysocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);

	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"网络错误！");
		CDialogEx::OnCancel();
	}

	switch (wEvent)
	{
	case FD_READ:
	{
		//接收服务器发送过来的消息包
		Msg_Pack msg = {};

		if (SOCKET_ERROR == recv(CTool::m_client.m_sClient, (char*)&msg, sizeof(Msg_Pack), NULL))
		{
			printf("recv error!\n");
			//return nullptr;
		}

		//根据消息的类型 , 选择不同的函数进行处理
		switch (msg.nMsgType)
		{
		case GetVirusMD5:
			RecvForVirusMD5(msg);
			break;
		case UploadVirusMD5:
			//RecvForGroupMsg(msg);
			break;
		}

	}

	}


	return 0;
}

void CKillVirus::RecvForVirusMD5(Msg_Pack msg)
{
	MessageBox(CString(msg.nMsgBuff));
}
