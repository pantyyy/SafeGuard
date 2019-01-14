#pragma once
#include "afxcmn.h"


// CThread 对话框

class CThread : public CDialogEx
{
	DECLARE_DYNAMIC(CThread)

public:
	CThread(DWORD pid , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThread();

// 对话框数据
	enum { IDD = IDD_DIALOG_Thread };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Thread;
	virtual BOOL OnInitDialog();
	DWORD nPid;
};
