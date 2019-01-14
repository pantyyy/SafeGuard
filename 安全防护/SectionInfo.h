#pragma once
#include "afxcmn.h"


// CSectionInfo 对话框

class CSectionInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionInfo)

public:
	CSectionInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSectionInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_SectionInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl SectionInfoList;
	virtual BOOL OnInitDialog();
};
