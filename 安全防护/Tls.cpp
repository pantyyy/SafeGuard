// Tls.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Tls.h"
#include "afxdialogex.h"
#include "Tool.h"

// CTls 对话框

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


// CTls 消息处理程序


BOOL CTls::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strTitle("TLS表");
	SetWindowText(strTitle.GetBuffer());
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	
	//延迟加载表的RVA
	DWORD dwTlsRVA = pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	//将RVA转成FOA
	DWORD dwTlsFOA = CTool::RVAtoFOA(dwTlsRVA, CTool::pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_TLS_DIRECTORY32 pTls = (PIMAGE_TLS_DIRECTORY32)(dwTlsFOA + CTool::pFileBuf);

	DataBegin.Format(L"%08X", pTls->StartAddressOfRawData);

	DataEnd.Format(L"%08X", pTls->EndAddressOfRawData);

	IndexVA.Format(L"%08X", pTls->AddressOfIndex);

	CallBackVA.Format(L"%08X", pTls->AddressOfCallBacks);

	SizeOfZero.Format(L"%08X", pTls->SizeOfZeroFill);

	FlagValue.Format(L"%08X", pTls->Characteristics);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
