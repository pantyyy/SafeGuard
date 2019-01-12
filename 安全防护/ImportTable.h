#pragma once
#include "afxcmn.h"


// CImportTable 对话框

class CImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTable)

public:
	CImportTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportTable();

// 对话框数据
	enum { IDD = IDD_DIALOG_ImportTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl ImportModel;
	CListCtrl ImportINT;
	CListCtrl ImportIAT;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
};
