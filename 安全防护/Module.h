#pragma once
#include "afxcmn.h"


// CModule 对话框

class CModule : public CDialogEx
{
	DECLARE_DYNAMIC(CModule)

public:
	CModule(DWORD pid , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModule();

// 对话框数据
	enum { IDD = IDD_DIALOG_Module };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Module;
	DWORD pid;
	virtual BOOL OnInitDialog();
};
