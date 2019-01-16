// KillVirus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
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

// CKillVirus �Ի���

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


// CKillVirus ��Ϣ�������


BOOL CKillVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString DlgTitle("������ɱ");
	SetWindowText(DlgTitle.GetBuffer());

	m_virus_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_virus_list.InsertColumn(0, L"��������", 0, 150);
	m_virus_list.InsertColumn(1, L"����·��", 0, 300);
	m_virus_list.InsertColumn(2, L"MD5", 0, 250);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CKillVirus::OnBnClickedButtonMd5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//1.��ȡָ��·�����е��ļ�
	m_virus_list.DeleteAllItems();
	std::vector<FileInfo> fileList;
	CTool::GetAllFile(m_path.GetBuffer(), fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"û���ҵ�����Ҫ����ļ�!");
		return;
	}

	//2.��ȡ���ر��ز���������
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
			////3.����MD5ֵ�ж��Ƿ��ǲ���
			if (strcmp(szBuff , local_md5[j]) == 0)
			{
				m_virus_list.InsertItem(nCount, L"");
				//������
				CString name(fileList[i].cFileName);
				m_virus_list.SetItemText(nCount, 0, name);

				//����·��
				CString path(fileList[i].cFilePath);
				m_virus_list.SetItemText(nCount, 1, path);

				//MD5ֵ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CKillVirus::OnBnClickedButtonWhiteList()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWhiteListKill* WhiteListKill = new CWhiteListKill();
	WhiteListKill->Create(IDD_DIALOG_White_List_Kill);
	WhiteListKill->ShowWindow(SW_SHOW);

}


void CKillVirus::OnBnClickedButtonCloudKill()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//���ӷ�����
	if (!CTool::m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"���ӷ�����ʧ�ܣ�", L"Error!", MB_OK || MB_ICONWARNING);
		return;
	}

	//���������ӳɹ�

	//����������ͻ�ȡ����MD5ֵ����Ϣ
	CStringA str("��ȡ����MD5");
	CTool::m_client.SendMsg(str.GetBuffer() , str.GetLength() , GetVirusMD5);

	//�������з��������͹���������
	WSAAsyncSelect(CTool::m_client.m_sClient, m_hWnd, WM_MYSOCKET, FD_READ | FD_CLOSE);

}


void CKillVirus::OnBnClickedButtonPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFolderPickerDialog dlg;
	dlg.DoModal();

	CString path = dlg.GetFolderPath();
	m_path = path;
	UpdateData(FALSE);
}


void CKillVirus::OnBnClickedButtonScan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_virus_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//����һ����Ž���ļ����Ƶ�����
	CTool::GetAllFile(m_path.GetBuffer(), fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"û���ҵ�����Ҫ����ļ�!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_virus_list.InsertItem(i, L"");
		//�ļ���
		CString name(fileList[i].cFileName);
		m_virus_list.SetItemText(i, 0, name);

		//�ļ�·��
		CString path(fileList[i].cFilePath);
		m_virus_list.SetItemText(i, 1, path);

		//�ļ�MD5ֵ
		//���ֽ�ת���ֽ�
		char* file_path_byte = new char[520];
		WideCharToMultiByte(CP_ACP, 0, fileList[i].cFilePath, -1, file_path_byte, 520, NULL, NULL);

		char* file_md5_str = md5FileValue(file_path_byte);

		//char* ת CString
		CString file_md5;
		file_md5.Format(L"%S", file_md5_str);
		m_virus_list.SetItemText(i, 2, file_md5);

	}
}


void CKillVirus::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�����˵�
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_Virus)); //���ز˵�
	HMENU hSub = GetSubMenu(hMenu, 0); //��ȡ�Ӳ˵����
	CPoint pt;
	GetCursorPos(&pt); //��ȡ����
	TrackPopupMenu(hSub, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);


	*pResult = 0;
}

//��Ӳ���������
void CKillVirus::OnVirus()
{
	// TODO:  �ڴ���������������
	//MessageBox(L"��Ӳ���");
	//��ȡѡ�еĽ���ID
	POSITION ps;
	int index;
	ps = m_virus_list.GetFirstSelectedItemPosition();
	//��ȡindex�±� , index���Ƕ�����
	index = m_virus_list.GetNextSelectedItem(ps);
	
	//��ȡѡ������ָ���е����� , 0��ʾ������
	CString virus_md5 = m_virus_list.GetItemText(index, 2);
	//���ֽ�ת���ֽ�
	char * virus_arr = new char[520];
	WideCharToMultiByte(CP_ACP, 0, virus_md5, -1, virus_arr, 520, NULL, NULL);

	//���ļ���д������
	ofstream ofresult("Virus_Database.txt", ios::app);
	ofresult << virus_arr << endl;
	MessageBox(L"��Ӳ����ɹ�!");
}


void CKillVirus::OnVirusUploadvirus()
{
	// TODO:  �ڴ���������������
	HANDLE hFile;//����һ�������   
	hFile = CreateFile(L"Virus_Database.txt",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);//ʹ��CreatFile���API�������ļ�   
	DWORD Written;
	WriteFile(hFile, "Welcome to VCFans!", strlen("Welcome to VCFans!"), &Written, NULL);//д���ļ�   
	CloseHandle(hFile);//�رվ��   
}


void CKillVirus::OnBnClickedButtonCleanVirus()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������



}


//���շ��������͹�������Ϣ
afx_msg LRESULT CKillVirus::OnMysocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);

	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"�������");
		CDialogEx::OnCancel();
	}

	switch (wEvent)
	{
	case FD_READ:
	{
		//���շ��������͹�������Ϣ��
		Msg_Pack msg = {};

		if (SOCKET_ERROR == recv(CTool::m_client.m_sClient, (char*)&msg, sizeof(Msg_Pack), NULL))
		{
			printf("recv error!\n");
			//return nullptr;
		}

		//������Ϣ������ , ѡ��ͬ�ĺ������д���
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
