#pragma once
#include "afxcmn.h"


// CRelocateTable �Ի���

class CRelocateTable : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocateTable)

public:
	CRelocateTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelocateTable();

// �Ի�������
	enum { IDD = IDD_DIALOG_RelocateTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl BlockList;
	CListCtrl ItemList;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
