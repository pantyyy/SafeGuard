#pragma once
#include "afxcmn.h"


// CExportTable 对话框

class CExportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CExportTable)

public:
	CExportTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportTable();

// 对话框数据
	enum { IDD = IDD_DIALOG_ExportTable };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
