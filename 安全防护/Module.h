#pragma once
#include "afxcmn.h"


// CModule �Ի���

class CModule : public CDialogEx
{
	DECLARE_DYNAMIC(CModule)

public:
	CModule(DWORD pid , CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModule();

// �Ի�������
	enum { IDD = IDD_DIALOG_Module };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Module;
	DWORD pid;
	virtual BOOL OnInitDialog();
};
