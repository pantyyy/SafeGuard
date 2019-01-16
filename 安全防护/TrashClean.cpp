// TrashClean.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "TrashClean.h"
#include "afxdialogex.h"
#include "Tool.h"
#include<vector>
using namespace std;
#include <Windows.h>
// CTrashClean 对话框

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


// CTrashClean 消息处理程序


void CTrashClean::OnBnClickedButtonTargetDir()
{
	// TODO:  在此添加控件通知处理程序代码
	CFolderPickerDialog dlg;
	dlg.DoModal();

	CString path = dlg.GetFolderPath();
	selected_path = path;
	UpdateData(FALSE);

}


void CTrashClean::OnBnClickedButtonScan()
{
	// TODO:  在此添加控件通知处理程序代码

	m_trash_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//定义一个存放结果文件名称的链表
	CTool::GetAllTrashFile(selected_path.GetBuffer() , fileList);

	if (fileList.size() == 0)
	{
		MessageBox(L"没有找到符合要求的文件!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_trash_list.InsertItem(i, L"");
		//文件名
		CString name(fileList[i].cFileName);
		m_trash_list.SetItemText(i, 0, name);

		//大小
		ULARGE_INTEGER fileSize;//记录文件大小
		ULARGE_INTEGER qwFileTotalSize = { 0 }; //文件总大小
		fileSize.HighPart = fileList[i].nFileSizeHigh;
		fileSize.LowPart = fileList[i].nFileSizeLow;
		qwFileTotalSize.QuadPart += fileSize.QuadPart;
		qwFileTotalSize.QuadPart = qwFileTotalSize.QuadPart / 1024;

		CString file_size;
		file_size.Format(L"%d", qwFileTotalSize.QuadPart);
		CString unit("KB");
		file_size = file_size + unit;
		m_trash_list.SetItemText(i, 1, file_size);

		//文件路径
		CString path(fileList[i].cFilePath);
		m_trash_list.SetItemText(i, 2, path);
	}

	file_list = fileList;
}


void CTrashClean::OnBnClickedButtonClean()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < file_list.size(); i++)
	{
		DeleteFile(file_list[i].cFilePath);
	}

	MessageBox(L"清理完毕！");
	m_trash_list.DeleteAllItems();
	UpdateData(FALSE);
}


BOOL CTrashClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置对话框名
	CString DlgTitle("垃圾清理");
	SetWindowText(DlgTitle.GetBuffer());

	m_trash_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//设置列表头
	m_trash_list.InsertColumn(0, L"文件名", 0, 150);
	m_trash_list.InsertColumn(1, L"大小", 0, 100);
	m_trash_list.InsertColumn(2, L"路径", 0, 550);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CTrashClean::OnBnClickedCheckWeb()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码

	m_trash_list.DeleteAllItems();
	std::vector<FileInfo> fileList;//定义一个存放结果文件名称的链表
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
		MessageBox(L"没有找到符合要求的文件!");
		return;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		m_trash_list.InsertItem(i, L"");
		//文件名
		CString name(fileList[i].cFileName);
		m_trash_list.SetItemText(i, 0, name);

		//大小
		ULARGE_INTEGER fileSize;//记录文件大小
		ULARGE_INTEGER qwFileTotalSize = { 0 }; //文件总大小
		fileSize.HighPart = fileList[i].nFileSizeHigh;
		fileSize.LowPart = fileList[i].nFileSizeLow;
		qwFileTotalSize.QuadPart += fileSize.QuadPart;
		qwFileTotalSize.QuadPart = qwFileTotalSize.QuadPart / 1024;

		CString file_size;
		file_size.Format(L"%d", qwFileTotalSize.QuadPart);
		CString unit("KB");
		file_size = file_size + unit;
		m_trash_list.SetItemText(i, 1, file_size);

		//文件路径
		CString path(fileList[i].cFilePath);
		m_trash_list.SetItemText(i, 2, path);
	}

}


void CTrashClean::OnBnClickedCheckSys()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_VS))->GetCheck())
	{
		vs_trash = 1;
	}
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_VS))->GetCheck())
	{
		vs_trash = 0;
	}
}
