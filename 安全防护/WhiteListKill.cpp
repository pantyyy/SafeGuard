// WhiteListKill.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
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

//��ȡ���н��̶���
vector<PROCESSENTRY32> GetAllProcess_White()
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

// CWhiteListKill �Ի���

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


// CWhiteListKill ��Ϣ�������


BOOL CWhiteListKill::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString WinTitle("��������ɱ");
	SetWindowText(WinTitle);

	//���ñ�ͷ
	m_white_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_white_list.InsertColumn(0, L"������������", 0, 200);
	m_white_list.InsertColumn(1, L"����ID", 0, 80);
	m_white_list.InsertColumn(2, L"������ID", 0, 80);
	m_white_list.InsertColumn(3, L"�߳���", 0, 80);
	m_white_list.InsertColumn(4, L"λ��", 0, 80);
	m_white_list.InsertColumn(5, L"·��", 0, 500);

	TravserVirusProcess();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CWhiteListKill::TravserVirusProcess()
{

	m_white_list.DeleteAllItems();

	//��ȡ�����б�
	vector<PROCESSENTRY32> process_list = GetAllProcess_White();

	//��ȡ�ļ���ָ���Ĳ�������
	vector<char*> local_process_list;
	char line[512] = { 0 };
	std::ifstream fin("White_List.txt", std::ios::in);
	while (fin.getline(line, sizeof(line))){
		char* temp = new char[520];
		memcpy(temp, line, sizeof(line));
		local_process_list.push_back(temp);
	}
	
	int nLine = 0;
	//���������б�
	for (int i = 0; i < process_list.size(); i++)
	{
		char* process_name = new char[520];
		WideCharToMultiByte(CP_ACP, 0, process_list[i].szExeFile, -1, process_name, 520, NULL, NULL);
		
		for (int k = 0; k < local_process_list.size(); k++)
		{
			if (strcmp(local_process_list[k], process_name) == 0)
			{
				
				int j = 0;

				//������
				m_white_list.InsertItem(nLine, process_list[i].szExeFile);

				//����ID
				CString c_id;
				c_id.Format(TEXT("%d"), process_list[i].th32ProcessID);
				m_white_list.SetItemText(nLine, ++j, c_id);

				//������ID
				CString p_id;
				p_id.Format(TEXT("%d"), process_list[i].th32ParentProcessID);
				m_white_list.SetItemText(nLine, ++j, p_id);

				//�߳���
				CString count_thread;
				count_thread.Format(TEXT("%d"), process_list[i].cntThreads);
				m_white_list.SetItemText(nLine , ++j, count_thread);

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
					m_white_list.SetItemText(nLine, ++j, L"32λ");
				else
					m_white_list.SetItemText(nLine, ++j, L"64λ");

				m_white_list.SetItemText(nLine , ++j, path);
			}
		}

	}

	virus_process_list = local_process_list;

	//�ͷ��ڴ�
	//for (int i = 0; i < local_process_list.size(); i++)
	//{
	//	delete local_process_list[i];
	//}
}


void CWhiteListKill::OnBnClickedButtonEndProcess()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_white_list.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_white_list.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString PID = m_white_list.GetItemText(index, 1);
	int ProcessID = _ttoi(PID);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
	if (TerminateProcess(hProcess, 1))
	{
		MessageBox(L"�������̳ɹ�!");
		TravserVirusProcess();
	}
	else{
		MessageBox(L"��������ʧ��!");
	}
	
	CloseHandle(hProcess);

}


void CWhiteListKill::OnBnClickedButtonAddToTrust()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_white_list.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_white_list.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString process_name = m_white_list.GetItemText(index, 0);

	//���ֽ�ת���ֽ�
	char * virus_str = new char[520];
	WideCharToMultiByte(CP_ACP, 0, process_name, -1, virus_str, 520, NULL, NULL);

	//д���ļ�
	ofstream ofresult("White_List.txt", ios::out);
	for (int i = 0; i < virus_process_list.size(); i++)
	{
		if (strcmp(virus_str, virus_process_list[i]) != 0)
		{
			ofresult << virus_process_list[i] << endl;
		}
		else{
			MessageBox(L"��������ӳɹ�!");
		}
	}

	TravserVirusProcess();
}
