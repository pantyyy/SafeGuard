// ResourceTable.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "ResourceTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CResourceTable 对话框

IMPLEMENT_DYNAMIC(CResourceTable, CDialogEx)

CResourceTable::CResourceTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResourceTable::IDD, pParent)
{

}

CResourceTable::~CResourceTable()
{
}

void CResourceTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, ResourceTree);
	DDX_Control(pDX, IDC_LIST1, ResourceList);
}


BEGIN_MESSAGE_MAP(CResourceTable, CDialogEx)
END_MESSAGE_MAP()


// CResourceTable 消息处理程序


BOOL CResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	ResourceTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	//获取NT头的指针
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//资源表的RVA
	DWORD dwResRVA = pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//RVA到FOA
	DWORD dwResFOA = CTool::RVAtoFOA(dwResRVA, CTool::pFileBuf);
	//资源表在文件中的起始位置
	PIMAGE_RESOURCE_DIRECTORY pResHeader = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CTool::pFileBuf);

	AddNode(pResHeader, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CResourceTable::AddNode(PIMAGE_RESOURCE_DIRECTORY pResHeader, HTREEITEM hParent)
{
	DWORD dwStringCount = pResHeader->NumberOfNamedEntries;
	DWORD dwNumberCount = pResHeader->NumberOfIdEntries;
	DWORD dwTotalCount = dwStringCount + dwNumberCount;

	for (int i = 0; i < dwTotalCount; i++)
	{

	}
}
