#pragma once
#include "afxcmn.h"


// CUnstall 对话框

class CUnstall : public CDialogEx
{
	DECLARE_DYNAMIC(CUnstall)

public:
	CUnstall(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUnstall();

// 对话框数据
	enum { IDD = IDD_DIALOG_Uninstall };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_uninstall_list;
	virtual BOOL OnInitDialog();
	void TraverseUninstallSoftware();
	afx_msg void OnUninstallSoftware();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
