#pragma once
#include "afxcmn.h"


// CDelay 对话框

class CDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDelay)

public:
	CDelay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDelay();

// 对话框数据
	enum { IDD = IDD_DIALOG_Delay };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_delay_1;
	CListCtrl m_delay_2;
	int nItem;
};
