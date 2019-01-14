#pragma once


// CMemoryManager 对话框

class CMemoryManager : public CDialogEx
{
	DECLARE_DYNAMIC(CMemoryManager)

public:
	CMemoryManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMemoryManager();

// 对话框数据
	enum { IDD = IDD_DIALOG_Memory_Manager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMemoryoptimize();
	CString m_CPU;
	CString m_Memory;
	virtual BOOL OnInitDialog();
};
