#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ClientSocket.h"

// CKillVirus 对话框

class CKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirus)

public:
	CKillVirus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKillVirus();

// 对话框数据
	enum { IDD = IDD_DIALOG_Kill_Virus };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_virus_list;
	CString m_path;
	afx_msg void OnBnClickedButtonMd5();
	afx_msg void OnBnClickedButtonKillByPath();
	afx_msg void OnBnClickedButtonWhiteList();
	afx_msg void OnBnClickedButtonCloudKill();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVirus();
	afx_msg void OnVirusUploadvirus();
	afx_msg void OnBnClickedButtonCleanVirus();

	void RecvForVirusMD5(Msg_Pack msg);
protected:
	afx_msg LRESULT OnMysocket(WPARAM wParam, LPARAM lParam);
};
