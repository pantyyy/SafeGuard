// Tls.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "Tls.h"
#include "afxdialogex.h"
#include "Tool.h"

// CTls �Ի���

IMPLEMENT_DYNAMIC(CTls, CDialogEx)

CTls::CTls(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTls::IDD, pParent)
	, DataBegin(_T(""))
	, DataEnd(_T(""))
	, IndexVA(_T(""))
	, CallBackVA(_T(""))
	, SizeOfZero(_T(""))
	, FlagValue(_T(""))
{

}

CTls::~CTls()
{
}

void CTls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, DataBegin);
	DDX_Text(pDX, IDC_EDIT4, DataEnd);
	DDX_Text(pDX, IDC_EDIT5, IndexVA);
	DDX_Text(pDX, IDC_EDIT18, CallBackVA);
	DDX_Text(pDX, IDC_EDIT19, SizeOfZero);
	DDX_Text(pDX, IDC_EDIT20, FlagValue);
}


BEGIN_MESSAGE_MAP(CTls, CDialogEx)
END_MESSAGE_MAP()


// CTls ��Ϣ�������


BOOL CTls::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strTitle("TLS��");
	SetWindowText(strTitle.GetBuffer());
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	
	//�ӳټ��ر��RVA
	DWORD dwTlsRVA = pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	//��RVAת��FOA
	DWORD dwTlsFOA = CTool::RVAtoFOA(dwTlsRVA, CTool::pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_TLS_DIRECTORY32 pTls = (PIMAGE_TLS_DIRECTORY32)(dwTlsFOA + CTool::pFileBuf);

	DataBegin.Format(L"%08X", pTls->StartAddressOfRawData);

	DataEnd.Format(L"%08X", pTls->EndAddressOfRawData);

	IndexVA.Format(L"%08X", pTls->AddressOfIndex);

	CallBackVA.Format(L"%08X", pTls->AddressOfCallBacks);

	SizeOfZero.Format(L"%08X", pTls->SizeOfZeroFill);

	FlagValue.Format(L"%08X", pTls->Characteristics);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
