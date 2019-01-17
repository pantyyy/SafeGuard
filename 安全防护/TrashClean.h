#pragma once
#include "afxcmn.h"
#include<vector>
#include "Tool.h"

// CTrashClean 对话框

class CTrashClean : public CDialogEx
{
	DECLARE_DYNAMIC(CTrashClean)

public:
	CTrashClean(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrashClean();

// 对话框数据
	enum { IDD = IDD_DIALOG_Trash_Clean };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTargetDir();
	CString selected_path;
	CListCtrl m_trash_list;
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonClean();
	virtual BOOL OnInitDialog();
	std::vector<CString> user_seleced_list;
	afx_msg void OnBnClickedCheckWeb();

	int web_trash;
	int sys_trash;
	int user_trash;
	int recycle_trash;
	int systemp_trash;
	int vs_trash;
	std::vector<FileInfo> file_list;
	afx_msg void OnBnClickedButtonOrientScan();
	afx_msg void OnBnClickedCheckSys();
	afx_msg void OnBnClickedCheckUser();
	afx_msg void OnBnClickedCheckRecycle();
	afx_msg void OnBnClickedCheckVs();
	afx_msg void OnBnClickedButtonCleanRecycleBin();
};
