#pragma once
#include "afxcmn.h"


// CResourceTable 对话框

class CResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceTable)

public:
	CResourceTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResourceTable();

// 对话框数据
	enum { IDD = IDD_DIALOG_Resource };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl ResourceTree;
	CListCtrl ResourceList;
	virtual BOOL OnInitDialog();
};
