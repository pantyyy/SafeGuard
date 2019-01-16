// TrashClean.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "TrashClean.h"
#include "afxdialogex.h"
#include "Tool.h"
#include<vector>
using namespace std;
#include <Windows.h>
// CTrashClean �Ի���

IMPLEMENT_DYNAMIC(CTrashClean, CDialogEx)

CTrashClean::CTrashClean(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrashClean::IDD, pParent)
	, selected_path(_T(""))
{

}

CTrashClean::~CTrashClean()
{
}

void CTrashClean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, selected_path);
	DDX_Control(pDX, IDC_LIST1, m_trash_list);
}


BEGIN_MESSAGE_MAP(CTrashClean, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Target_Dir, &CTrashClean::OnBnClickedButtonTargetDir)
	ON_BN_CLICKED(IDC_BUTTON_Scan, &CTrashClean::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_Clean, &CTrashClean::OnBnClickedButtonClean)
	ON_BN_CLICKED(IDC_CHECK_Web, &CTrashClean::OnBnClickedCheckWeb)
	ON_BN_CLICKED(IDC_BUTTON_Orient_Scan, &CTrashClean::OnBnClickedButtonOrientScan)
	ON_BN_CLICKED(IDC_CHECK_Sys, &CTrashClean::OnBnClickedCheckSys)
	ON_BN_CLICKED(IDC_CHECK_User, &CTrashClean::OnBnClickedCheckUser)
	ON_BN_CLICKED(IDC_CHECK_Recycle, &CTrashClean::OnBnClickedCheckRecycle)
	ON_BN_CLICKED(IDC_CHECK_VS, &CTrashClean::OnBnClickedCheckVs)
END_MESSAGE_MAP()


// CTrashClean ��Ϣ�������


void CTrashClean::OnBnClickedButtonTargetDir()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFolderPickerDialog dlg;
	dlg.DoModal();

	CString path = dlg.GetFolderPath();
	selected_path = path;
	UpdateData(FALSE);

}


void CTrashClean::OnBnClickedButtonScan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_trash_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//����һ����Ž���ļ����Ƶ�����
	CTool::GetAllTrashFile(selected_path.GetBuffer() , fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"û���ҵ�����Ҫ����ļ�!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_trash_list.InsertItem(i, L"");
		//�ļ���
		CString name(fileList[i].cFileName);
		m_trash_list.SetItemText(i, 0, name);

		//��С
		ULARGE_INTEGER fileSize;//��¼�ļ���С
		ULARGE_INTEGER qwFileTotalSize = { 0 }; //�ļ��ܴ�С
		fileSize.HighPart = fileList[i].nFileSizeHigh;
		fileSize.LowPart = fileList[i].nFileSizeLow;
		qwFileTotalSize.QuadPart += fileSize.QuadPart;
		qwFileTotalSize.QuadPart = qwFileTotalSize.QuadPart / 1024;

		CString file_size;
		file_size.Format(L"%d", qwFileTotalSize.QuadPart);
		CString unit("KB");
		file_size = file_size + unit;
		m_trash_list.SetItemText(i, 1, file_size);

		//�ļ�·��
		CString path(fileList[i].cFilePath);
		m_trash_list.SetItemText(i, 2, path);
	}

	file_list = fileList;
}


void CTrashClean::OnBnClickedButtonClean()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < file_list.size(); i++)
	{
		DeleteFile(file_list[i].cFilePath);
	}

	MessageBox(L"������ϣ�");
	m_trash_list.DeleteAllItems();
	UpdateData(FALSE);
}


BOOL CTrashClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���öԻ�����
	CString DlgTitle("��������");
	SetWindowText(DlgTitle.GetBuffer());

	m_trash_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//�����б�ͷ
	m_trash_list.InsertColumn(0, L"�ļ���", 0, 150);
	m_trash_list.InsertColumn(1, L"��С", 0, 100);
	m_trash_list.InsertColumn(2, L"·��", 0, 550);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CTrashClean::OnBnClickedCheckWeb()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Web))->GetCheck())
	{
		web_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Web))->GetCheck())
	{
		web_trash = 0;
	}
}


void CTrashClean::OnBnClickedButtonOrientScan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_trash_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//����һ����Ž���ļ����Ƶ�����
	if (web_trash == 1)
	{
		CTool::GetAllTrashFile(L"C:\\Users\\Neo\\AppData\\Local\\Microsoft\\Windows\\WebCache", fileList);
		CTool::GetAllTrashFile(L"C:\\Users\\Neo\\AppData\\Local\\Microsoft\\Windows\\WebCache", fileList);
		CTool::GetAllTrashFile(L"C:\\Users\\Neo\\AppData\\Local\\Google\\Chrome\\User Data", fileList);
	}

	if (sys_trash == 1)
	{
		CTool::GetAllTrashFile(L"C:\\Windows\\Temp", fileList);
	}

	if (user_trash == 1)
	{
		CTool::GetAllTrashFile(L"C:\\Users\\Neo\\AppData\\Local\\Temp", fileList);
	}

	if (vs_trash == 1)
	{
		CTool::GetAllTrashFile(L"D:\\15PB\\15PBproject", fileList);
	}


	if (fileList.size() == 0)
	{
		MessageBox(L"û���ҵ�����Ҫ����ļ�!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_trash_list.InsertItem(i, L"");
		//�ļ���
		CString name(fileList[i].cFileName);
		m_trash_list.SetItemText(i, 0, name);

		//��С
		ULARGE_INTEGER fileSize;//��¼�ļ���С
		ULARGE_INTEGER qwFileTotalSize = { 0 }; //�ļ��ܴ�С
		fileSize.HighPart = fileList[i].nFileSizeHigh;
		fileSize.LowPart = fileList[i].nFileSizeLow;
		qwFileTotalSize.QuadPart += fileSize.QuadPart;
		qwFileTotalSize.QuadPart = qwFileTotalSize.QuadPart / 1024;

		CString file_size;
		file_size.Format(L"%d", qwFileTotalSize.QuadPart);
		CString unit("KB");
		file_size = file_size + unit;
		m_trash_list.SetItemText(i, 1, file_size);

		//�ļ�·��
		CString path(fileList[i].cFilePath);
		m_trash_list.SetItemText(i, 2, path);
	}

}


void CTrashClean::OnBnClickedCheckSys()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Sys))->GetCheck())
	{
		sys_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Sys))->GetCheck())
	{
		sys_trash = 0;
	}
}


void CTrashClean::OnBnClickedCheckUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_User))->GetCheck())
	{
		user_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_User))->GetCheck())
	{
		user_trash = 0;
	}
}


void CTrashClean::OnBnClickedCheckRecycle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Recycle))->GetCheck())
	{
		recycle_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_Recycle))->GetCheck())
	{
		recycle_trash = 0;
	}
}


void CTrashClean::OnBnClickedCheckVs()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_VS))->GetCheck())
	{
		vs_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_VS))->GetCheck())
	{
		vs_trash = 0;
	}
}
