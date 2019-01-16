#pragma once
#include "afxcmn.h"
#include<windows.h>
#include<TlHelp32.h>
#include<vector>
using std::vector;

// CWhiteListKill 对话框

class CWhiteListKill : public CDialogEx
{
	DECLARE_DYNAMIC(CWhiteListKill)

public:
	CWhiteListKill(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWhiteListKill();

// 对话框数据
	enum { IDD = IDD_DIALOG_White_List_Kill };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_white_list;
	virtual BOOL OnInitDialog();
	void TravserVirusProcess();
	afx_msg void OnBnClickedButtonEndProcess();
	afx_msg void OnBnClickedButtonAddToTrust();

	vector<char*> virus_process_list;
};
