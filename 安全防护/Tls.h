#pragma once


// CTls �Ի���

class CTls : public CDialogEx
{
	DECLARE_DYNAMIC(CTls)

public:
	CTls(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTls();

// �Ի�������
	enum { IDD = IDD_DIALOG_TLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
