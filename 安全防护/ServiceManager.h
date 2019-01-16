#pragma once
#include "afxcmn.h"


// CServiceManager �Ի���

class CServiceManager : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceManager)

public:
	CServiceManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceManager();
	void TraverseService();
// �Ի�������
	enum { IDD = IDD_DIALOG_ServiceManager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_service;
	virtual BOOL OnInitDialog();
	afx_msg void OnServiceClose();
	afx_msg void OnServiceStart();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
