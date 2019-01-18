#pragma once


// CTls 对话框

class CTls : public CDialogEx
{
	DECLARE_DYNAMIC(CTls)

public:
	CTls(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTls();

// 对话框数据
	enum { IDD = IDD_DIALOG_TLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString DataBegin;
	CString DataEnd;
	CString IndexVA;
	CString CallBackVA;
	CString SizeOfZero;
	CString FlagValue;
	virtual BOOL OnInitDialog();
};
