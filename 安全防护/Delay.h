#pragma once
#include "afxcmn.h"


// CDelay �Ի���

class CDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDelay)

public:
	CDelay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDelay();

// �Ի�������
	enum { IDD = IDD_DIALOG_Delay };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_delay_1;
	CListCtrl m_delay_2;
	int nItem;
};
