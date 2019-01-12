#pragma once
#include "afxcmn.h"


// CExportTable �Ի���

class CExportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CExportTable)

public:
	CExportTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportTable();

// �Ի�������
	enum { IDD = IDD_DIALOG_ExportTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString ExportTableRVA;
	CString NumberOfFuns;
	CString AddressOfNameTable;
	CString BaseOfOrdinal;
	CString NumberOfNamefuns;
	CString AddressOfOrdinalTable;
	CString FeatureValue;
	CString AddressOfAddressTable;
	CString Name;
	CString ModelName;
	CListCtrl ExportTableList;
	virtual BOOL OnInitDialog();
};
