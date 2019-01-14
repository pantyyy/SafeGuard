// DirInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "DirInfo.h"
#include "afxdialogex.h"
#include "Tool.h"

// CDirInfo 对话框

IMPLEMENT_DYNAMIC(CDirInfo, CDialogEx)

CDirInfo::CDirInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirInfo::IDD, pParent)
	, A1(_T(""))
	, A2(_T(""))
	, B1(_T(""))
	, B2(_T(""))
	, C1(_T(""))
	, C2(_T(""))
	, D1(_T(""))
	, D2(_T(""))
	, E1(_T(""))
	, E2(_T(""))
	, F1(_T(""))
	, F2(_T(""))
	, G1(_T(""))
	, G2(_T(""))
	, H1(_T(""))
	, H2(_T(""))
	, I1(_T(""))
	, I2(_T(""))
	, J1(_T(""))
	, J2(_T(""))
	, K1(_T(""))
	, K2(_T(""))
	, L1(_T(""))
	, L2(_T(""))
	, M1(_T(""))
	, M2(_T(""))
	, N1(_T(""))
	, N2(_T(""))
{

}

CDirInfo::~CDirInfo()
{
}

void CDirInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, A1);
	DDX_Text(pDX, IDC_EDIT1, A2);
	DDX_Text(pDX, IDC_EDIT18, B1);
	DDX_Text(pDX, IDC_EDIT5, B2);
	DDX_Text(pDX, IDC_EDIT20, C1);
	DDX_Text(pDX, IDC_EDIT19, C2);
	DDX_Text(pDX, IDC_EDIT22, D1);
	DDX_Text(pDX, IDC_EDIT21, D2);
	DDX_Text(pDX, IDC_EDIT24, E1);
	DDX_Text(pDX, IDC_EDIT23, E2);
	DDX_Text(pDX, IDC_EDIT26, F1);
	DDX_Text(pDX, IDC_EDIT25, F2);
	DDX_Text(pDX, IDC_EDIT28, G1);
	DDX_Text(pDX, IDC_EDIT27, G2);
	DDX_Text(pDX, IDC_EDIT30, H1);
	DDX_Text(pDX, IDC_EDIT29, H2);
	DDX_Text(pDX, IDC_EDIT36, I1);
	DDX_Text(pDX, IDC_EDIT35, I2);
	DDX_Text(pDX, IDC_EDIT38, J1);
	DDX_Text(pDX, IDC_EDIT37, J2);
	DDX_Text(pDX, IDC_EDIT40, K1);
	DDX_Text(pDX, IDC_EDIT39, K2);
	DDX_Text(pDX, IDC_EDIT42, L1);
	DDX_Text(pDX, IDC_EDIT41, L2);
	DDX_Text(pDX, IDC_EDIT44, M1);
	DDX_Text(pDX, IDC_EDIT43, M2);
	DDX_Text(pDX, IDC_EDIT46, N1);
	DDX_Text(pDX, IDC_EDIT45, N2);
}


BEGIN_MESSAGE_MAP(CDirInfo, CDialogEx)
END_MESSAGE_MAP()


// CDirInfo 消息处理程序


BOOL CDirInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	A1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[0].VirtualAddress);
	A2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[0].Size);
	B1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[1].VirtualAddress);
	B2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[1].Size);
	C1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[2].VirtualAddress);
	C2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[2].Size);
	D1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[3].VirtualAddress);
	D2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[3].Size);
	E1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[4].VirtualAddress);
	E2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[4].Size);
	F1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[5].VirtualAddress);
	F2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[5].Size);
	G1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[6].VirtualAddress);
	G2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[6].Size);
	H1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[7].VirtualAddress);
	H2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[7].Size);
	I1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[8].VirtualAddress);
	I2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[8].Size);
	J1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[9].VirtualAddress);
	J2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[9].Size);
	K1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[10].VirtualAddress);
	K2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[10].Size);
	L1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[11].VirtualAddress);
	L2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[11].Size);
	M1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[12].VirtualAddress);
	M2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[12].Size);
	N1.Format(L"%08X", pNt->OptionalHeader.DataDirectory[13].VirtualAddress);
	N2.Format(L"%08X", pNt->OptionalHeader.DataDirectory[13].Size);

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
