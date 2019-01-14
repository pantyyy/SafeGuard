#pragma once
#include "afxcmn.h"


// CTaskManager �Ի���

class CTaskManager : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskManager)

public:
	CTaskManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskManager();

// �Ի�������
	enum { IDD = IDD_DIALOG_TaskManager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int winCount;
	virtual BOOL OnInitDialog();
	CListCtrl m_Process;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProcessThread();
	afx_msg void OnProcessModel();
	afx_msg void OnProcessTerminateprocess();
	afx_msg void OnBnClickedButtonShowwin();
	afx_msg void OnBnClickedButtonMemoryControl();
};
