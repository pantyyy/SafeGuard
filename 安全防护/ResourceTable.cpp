// ResourceTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ResourceTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CResourceTable �Ի���

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


// CResourceTable ��Ϣ�������


BOOL CResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	ResourceTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	//��ȡNTͷ��ָ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//��Դ���RVA
	DWORD dwResRVA = pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//RVA��FOA
	DWORD dwResFOA = CTool::RVAtoFOA(dwResRVA, CTool::pFileBuf);
	//��Դ�����ļ��е���ʼλ��
	PIMAGE_RESOURCE_DIRECTORY pResHeader = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CTool::pFileBuf);

	AddNode(pResHeader, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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
