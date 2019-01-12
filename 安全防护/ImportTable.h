#pragma once
#include "afxcmn.h"


// CImportTable �Ի���

class CImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTable)

public:
	CImportTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportTable();

// �Ի�������
	enum { IDD = IDD_DIALOG_ImportTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl ImportModel;
	CListCtrl ImportINT;
	CListCtrl ImportIAT;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
};
