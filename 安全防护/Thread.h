#pragma once
#include "afxcmn.h"


// CThread �Ի���

class CThread : public CDialogEx
{
	DECLARE_DYNAMIC(CThread)

public:
	CThread(DWORD pid , CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThread();

// �Ի�������
	enum { IDD = IDD_DIALOG_Thread };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Thread;
	virtual BOOL OnInitDialog();
	DWORD nPid;
};
