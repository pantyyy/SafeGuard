
// ��ȫ����Dlg.h : ͷ�ļ�
//

#pragma once


// C��ȫ����Dlg �Ի���
class C��ȫ����Dlg : public CDialogEx
{
// ����
public:
	C��ȫ����Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPe();
	afx_msg void OnBnClickedButtonTaskmanager();
	afx_msg void OnBnClickedButtonCleantrash();
};
