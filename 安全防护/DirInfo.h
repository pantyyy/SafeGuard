#pragma once


// CDirInfo 对话框

class CDirInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDirInfo)

public:
	CDirInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_Dir };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString A1;
	CString A2;
	CString B1;
	CString B2;
	CString C1;
	CString C2;
	CString D1;
	CString D2;
	CString E1;
	CString E2;
	CString F1;
	CString F2;
	CString G1;
	CString G2;
	CString H1;
	CString H2;
	CString I1;
	CString I2;
	CString J1;
	CString J2;
	CString K1;
	CString K2;
	CString L1;
	CString L2;
	CString M1;
	CString M2;
	CString N1;
	CString N2;
	virtual BOOL OnInitDialog();
};
