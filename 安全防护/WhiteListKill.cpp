// WhiteListKill.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "WhiteListKill.h"
#include "afxdialogex.h"

#include<windows.h>
#include<TlHelp32.h>
#include<vector>
using std::vector;

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

//获取所有进程对象
vector<PROCESSENTRY32> GetAllProcess_White()
{
	vector<PROCESSENTRY32> process_list;
	//1.创建进程快照
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	//2.进程结构体 , 用来保存遍历到的每一个进程
	PROCESSENTRY32 process_info = { sizeof(PROCESSENTRY32) };

	//3.尝试获取快照中的第一个进程 , 判断是否成功
	if (Process32First(snapshot, &process_info))
	{
		//输入获取到的进程信息
		do
		{
			process_list.push_back(process_info);

		} while (Process32Next(snapshot, &process_info));//获取下一个进程信息
	}

	return process_list;
}

// CWhiteListKill 对话框

IMPLEMENT_DYNAMIC(CWhiteListKill, CDialogEx)

CWhiteListKill::CWhiteListKill(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWhiteListKill::IDD, pParent)
{

}

CWhiteListKill::~CWhiteListKill()
{
}

void CWhiteListKill::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_white_list);
}


BEGIN_MESSAGE_MAP(CWhiteListKill, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_End_Process, &CWhiteListKill::OnBnClickedButtonEndProcess)
	ON_BN_CLICKED(IDC_BUTTON_Add_To_Trust, &CWhiteListKill::OnBnClickedButtonAddToTrust)
END_MESSAGE_MAP()


// CWhiteListKill 消息处理程序


BOOL CWhiteListKill::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString WinTitle("白名单查杀");
	SetWindowText(WinTitle);

	//设置表头
	m_white_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_white_list.InsertColumn(0, L"病毒进程名称", 0, 200);
	m_white_list.InsertColumn(1, L"进程ID", 0, 80);
	m_white_list.InsertColumn(2, L"父进程ID", 0, 80);
	m_white_list.InsertColumn(3, L"线程数", 0, 80);
	m_white_list.InsertColumn(4, L"位数", 0, 80);
	m_white_list.InsertColumn(5, L"路径", 0, 500);

	TravserVirusProcess();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CWhiteListKill::TravserVirusProcess()
{

	m_white_list.DeleteAllItems();

	//获取进程列表
	vector<PROCESSENTRY32> process_list = GetAllProcess_White();

	//获取文件中指定的病毒进程
	vector<char*> local_process_list;
	char line[512] = { 0 };
	std::ifstream fin("White_List.txt", std::ios::in);
	while (fin.getline(line, sizeof(line))){
		char* temp = new char[520];
		memcpy(temp, line, sizeof(line));
		local_process_list.push_back(temp);
	}
	
	int nLine = 0;
	//遍历进程列表
	for (int i = 0; i < process_list.size(); i++)
	{
		char* process_name = new char[520];
		WideCharToMultiByte(CP_ACP, 0, process_list[i].szExeFile, -1, process_name, 520, NULL, NULL);
		
		for (int k = 0; k < local_process_list.size(); k++)
		{
			if (strcmp(local_process_list[k], process_name) == 0)
			{
				
				int j = 0;

				//进程名
				m_white_list.InsertItem(nLine, process_list[i].szExeFile);

				//进程ID
				CString c_id;
				c_id.Format(TEXT("%d"), process_list[i].th32ProcessID);
				m_white_list.SetItemText(nLine, ++j, c_id);

				//父进程ID
				CString p_id;
				p_id.Format(TEXT("%d"), process_list[i].th32ParentProcessID);
				m_white_list.SetItemText(nLine, ++j, p_id);

				//线程数
				CString count_thread;
				count_thread.Format(TEXT("%d"), process_list[i].cntThreads);
				m_white_list.SetItemText(nLine , ++j, count_thread);

				//进程完整路径
				//1.获取进程句柄
				HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_list[i].th32ProcessID);
				//2.创建数组保存路径
				DWORD length = MAX_PATH;
				WCHAR path[MAX_PATH] = { 0 };
				//3.输入完整路径
				QueryFullProcessImageName(process, 0, path, &length);


				BOOL Is32 = FALSE;
				//64 - bit process on 64 - bit Windows : FALSE
				//32 - bit process on 64 - bit Windows : TRUE
				IsWow64Process(process, &Is32);
				if (Is32)
					m_white_list.SetItemText(nLine, ++j, L"32位");
				else
					m_white_list.SetItemText(nLine, ++j, L"64位");

				m_white_list.SetItemText(nLine , ++j, path);
			}
		}

	}

	virus_process_list = local_process_list;

	//释放内存
	//for (int i = 0; i < local_process_list.size(); i++)
	//{
	//	delete local_process_list[i];
	//}
}


void CWhiteListKill::OnBnClickedButtonEndProcess()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_white_list.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_white_list.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString PID = m_white_list.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
	if (TerminateProcess(hProcess, 1))
	{
		MessageBox(L"结束进程成功!");
		TravserVirusProcess();
	}
	else{
		MessageBox(L"结束进程失败!");
	}
	
	CloseHandle(hProcess);

}


void CWhiteListKill::OnBnClickedButtonAddToTrust()
{
	// TODO:  在此添加控件通知处理程序代码

	//获取选中的进程ID
	POSITION ps;
	int index;
	ps = m_white_list.GetFirstSelectedItemPosition();
	//获取index下标 , index即是多少行
	index = m_white_list.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据 , 0表示多少列
	CString process_name = m_white_list.GetItemText(index, 0);

	//宽字节转多字节
	char * virus_str = new char[520];
	WideCharToMultiByte(CP_ACP, 0, process_name, -1, virus_str, 520, NULL, NULL);

	//写入文件
	ofstream ofresult("White_List.txt", ios::out);
	for (int i = 0; i < virus_process_list.size(); i++)
	{
		if (strcmp(virus_str, virus_process_list[i]) != 0)
		{
			ofresult << virus_process_list[i] << endl;
		}
		else{
			MessageBox(L"白名单添加成功!");
		}
	}

	TravserVirusProcess();
}
