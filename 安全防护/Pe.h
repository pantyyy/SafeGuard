#pragma once


// CPe �Ի���

class CPe : public CDialogEx
{
	DECLARE_DYNAMIC(CPe)

public:
	CPe(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPe();
	enum { IDD = IDD_DIALOG_PE };

	CString FilePath;
	CString AddressOfEntryPoint;
	CString ImageBase;
	CString SizeOfImage;
	CString BaseOfCode;
	CString BaseOfData;
	CString SectionAligment;
	CString FileAlignment;
	CString Magic;
	CString Subsystem;
	CString NumberOfSections;
	CString TimeDataStamp;
	CString SizeOfHeaders;
	CString DllCharacteristics;
	CString CheckSum;
	CString SizeOfOptionalHeader;
	CString NumberOfRvaAndSizes;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectpath();
	afx_msg void OnBnClickedButtonResolve();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonResourcetable();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonRelocate();
	afx_msg void OnBnClickedButtonSection();
	afx_msg void OnBnClickedButtonDir();
};
