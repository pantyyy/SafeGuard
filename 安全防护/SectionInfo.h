#pragma once
#include "afxcmn.h"


// CSectionInfo �Ի���

class CSectionInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionInfo)

public:
	CSectionInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_SectionInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl SectionInfoList;
	virtual BOOL OnInitDialog();
};
