// Module.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "Module.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <Windows.h>
#include <TlHelp32.h>

// CModule �Ի���

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


// CModule ��Ϣ�������


BOOL CModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���öԻ�����
	CString DlgTitle("ģ����Ϣ");
	SetWindowText(DlgTitle.GetBuffer());

	m_Module.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Module.InsertColumn(0, L"ģ������", 0, 120);
	m_Module.InsertColumn(1, L"ģ����ػ�ַ", 0, 120);
	m_Module.InsertColumn(2, L"ģ��ռ���ڴ��С", 0, 120);
	m_Module.InsertColumn(3, L"ģ����ڵ�ַ", 0, 120);

	//1.����ģ�����
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"û��ģ��");
	}
	else{
		//2.����ģ����� , ������Ϣ
		MODULEENTRY32 ModuleInfo = { sizeof(MODULEENTRY32) };

		//3.����ģ��
		int nCount = 0;
		if (Module32First(Snapshot, &ModuleInfo))
		{
			do 
			{
				if (ModuleInfo.th32ProcessID == pid)
				{
					MODULEINFO stcModuleInfo = { 0 };
					m_Module.InsertItem(nCount, L"");
					//stcModuleInfo��д��Ϣ
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
					GetModuleInformation(hProcess, ModuleInfo.hModule, &stcModuleInfo, sizeof(stcModuleInfo));

					//ģ����
					m_Module.SetItemText(nCount, 0, ModuleInfo.szModule);

					//ģ����ػ�ַ
					CString tempStr;
					tempStr.Format(_T("%x"), stcModuleInfo.lpBaseOfDll);
					m_Module.SetItemText(nCount, 1, tempStr);

					//ģ��ռ���ڴ��С
					tempStr.Format(_T("%d"), stcModuleInfo.SizeOfImage);
					m_Module.SetItemText(nCount, 2, tempStr);

					//ģ����ڵ�ַ
					tempStr.Format(_T("%x"), stcModuleInfo.EntryPoint);
					m_Module.SetItemText(nCount, 3, tempStr);

					nCount++;
				}
			} while (Module32Next(Snapshot, &ModuleInfo));

		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
