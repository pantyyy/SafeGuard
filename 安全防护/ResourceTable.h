#pragma once
#include "afxcmn.h"
#include "ResourceDescriptor.h"
#include <unordered_map>

// CResourceTable 对话框

class CResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceTable)

public:
	CResourceTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResourceTable();

// 对话框数据
	enum { IDD = IDD_DIALOG_Resource };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl ResourceTree;

	unordered_map<HTREEITEM, CResourceDescriptor *> m_TreeDataMap;
	virtual BOOL OnInitDialog();
	void DecodeResourceEntry();
	void GetNodeName(CResourceDescriptor *pDescriptor, CString& str);
	void GetInternalDisplayName(WORD wType, CString & str);
	void AddNode(CResourceDescriptor * pDescriptor, HTREEITEM hParent);
	void _DecodeResourceDirEntry(CResourceDescriptor * pDirNode, PIMAGE_RESOURCE_DIRECTORY pDirInfo);
	CResourceDescriptor *m_pResourceTable;
	CString num_of_str;
	CString num_of_ID;
	CString node_type;
	CString rva;
	CString foa;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
