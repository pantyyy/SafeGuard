#pragma once


// CMemoryManager �Ի���

class CMemoryManager : public CDialogEx
{
	DECLARE_DYNAMIC(CMemoryManager)

public:
	CMemoryManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMemoryManager();

// �Ի�������
	enum { IDD = IDD_DIALOG_Memory_Manager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMemoryoptimize();
	CString m_CPU;
	CString m_Memory;
	virtual BOOL OnInitDialog();
};
