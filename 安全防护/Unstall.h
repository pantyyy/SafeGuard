#pragma once
#include "afxcmn.h"


// CUnstall �Ի���

class CUnstall : public CDialogEx
{
	DECLARE_DYNAMIC(CUnstall)

public:
	CUnstall(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUnstall();

// �Ի�������
	enum { IDD = IDD_DIALOG_Uninstall };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_uninstall_list;
	virtual BOOL OnInitDialog();
	void TraverseUninstallSoftware();
	afx_msg void OnUninstallSoftware();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
