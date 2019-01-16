#pragma once
#include "afxcmn.h"


// CServiceManager 对话框

class CServiceManager : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceManager)

public:
	CServiceManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServiceManager();
	void TraverseService();
// 对话框数据
	enum { IDD = IDD_DIALOG_ServiceManager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_service;
	virtual BOOL OnInitDialog();
	afx_msg void OnServiceClose();
	afx_msg void OnServiceStart();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
