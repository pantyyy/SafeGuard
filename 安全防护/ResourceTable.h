#pragma once
#include "afxcmn.h"


// CResourceTable �Ի���

class CResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceTable)

public:
	CResourceTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResourceTable();

// �Ի�������
	enum { IDD = IDD_DIALOG_Resource };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl ResourceTree;
	CListCtrl ResourceList;
	virtual BOOL OnInitDialog();
};
