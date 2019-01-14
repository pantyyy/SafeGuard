#pragma once
#include "afxcmn.h"


// CRelocateTable 对话框

class CRelocateTable : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocateTable)

public:
	CRelocateTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelocateTable();

// 对话框数据
	enum { IDD = IDD_DIALOG_RelocateTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl BlockList;
	CListCtrl ItemList;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
