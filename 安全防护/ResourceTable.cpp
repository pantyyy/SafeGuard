// ResourceTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ResourceTable.h"
#include "afxdialogex.h"


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
