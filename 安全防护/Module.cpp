// Module.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Module.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <Windows.h>
#include <TlHelp32.h>

// CModule 对话框

IMPLEMENT_DYNAMIC(CModule, CDialogEx)

CModule::CModule(DWORD pid, CWnd* pParent /*=NULL*/)
	: CDialogEx(CModule::IDD, pParent)
{
	this->pid = pid;
}

CModule::~CModule()
{
}

void CModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Module);
}


BEGIN_MESSAGE_MAP(CModule, CDialogEx)
END_MESSAGE_MAP()


// CModule 消息处理程序


BOOL CModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框名
	CString DlgTitle("模块信息");
	SetWindowText(DlgTitle.GetBuffer());

	m_Module.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Module.InsertColumn(0, L"模块名称", 0, 120);
	m_Module.InsertColumn(1, L"模块加载基址", 0, 120);
	m_Module.InsertColumn(2, L"模块占用内存大小", 0, 120);
	m_Module.InsertColumn(3, L"模块入口地址", 0, 120);

	//1.创建模块快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"没有模块");
	}
	else{
		//2.创建模块对象 , 保存信息
		MODULEENTRY32 ModuleInfo = { sizeof(MODULEENTRY32) };

		//3.遍历模块
		int nCount = 0;
		if (Module32First(Snapshot, &ModuleInfo))
		{
			do 
			{
				if (ModuleInfo.th32ProcessID == pid)
				{
					MODULEINFO stcModuleInfo = { 0 };
					m_Module.InsertItem(nCount, L"");
					//stcModuleInfo填写信息
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
					GetModuleInformation(hProcess, ModuleInfo.hModule, &stcModuleInfo, sizeof(stcModuleInfo));

					//模块名
					m_Module.SetItemText(nCount, 0, ModuleInfo.szModule);

					//模块加载基址
					CString tempStr;
					tempStr.Format(_T("%x"), stcModuleInfo.lpBaseOfDll);
					m_Module.SetItemText(nCount, 1, tempStr);

					//模块占用内存大小
					tempStr.Format(_T("%d"), stcModuleInfo.SizeOfImage);
					m_Module.SetItemText(nCount, 2, tempStr);

					//模块入口地址
					tempStr.Format(_T("%x"), stcModuleInfo.EntryPoint);
					m_Module.SetItemText(nCount, 3, tempStr);

					nCount++;
				}
			} while (Module32Next(Snapshot, &ModuleInfo));

		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
