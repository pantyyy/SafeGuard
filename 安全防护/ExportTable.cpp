// ExportTable.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "ExportTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CExportTable 对话框

IMPLEMENT_DYNAMIC(CExportTable, CDialogEx)

CExportTable::CExportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExportTable::IDD, pParent)
	, ExportTableRVA(_T(""))
	, NumberOfFuns(_T(""))
	, AddressOfNameTable(_T(""))
	, BaseOfOrdinal(_T(""))
	, NumberOfNamefuns(_T(""))
	, AddressOfOrdinalTable(_T(""))
	, FeatureValue(_T(""))
	, AddressOfAddressTable(_T(""))
	, Name(_T(""))
	, ModelName(_T(""))
{

}

CExportTable::~CExportTable()
{
}

void CExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ExportTableRVA);
	DDX_Text(pDX, IDC_EDIT19, NumberOfFuns);
	DDX_Text(pDX, IDC_EDIT23, AddressOfNameTable);
	DDX_Text(pDX, IDC_EDIT4, BaseOfOrdinal);
	DDX_Text(pDX, IDC_EDIT20, NumberOfNamefuns);
	DDX_Text(pDX, IDC_EDIT24, AddressOfOrdinalTable);
	DDX_Text(pDX, IDC_EDIT5, FeatureValue);
	DDX_Text(pDX, IDC_EDIT21, AddressOfAddressTable);
	DDX_Text(pDX, IDC_EDIT18, Name);
	DDX_Text(pDX, IDC_EDIT22, ModelName);
	DDX_Control(pDX, IDC_LIST2, ExportTableList);
}


BEGIN_MESSAGE_MAP(CExportTable, CDialogEx)
END_MESSAGE_MAP()


// CExportTable 消息处理程序


BOOL CExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//设置对话框名
	CString DlgTitle("导出表");
	SetWindowText(DlgTitle.GetBuffer());

	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//获取导出表的RVA
	DWORD dwExportRVA = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//RVA转化为FOA
	DWORD dwExportFOA = CTool::RVAtoFOA(dwExportRVA, CTool::pFileBuf);
	//导出表对象
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(CTool::pFileBuf + dwExportFOA);

	//上部分信息
	//输出表RVA
	ExportTableRVA.Format(L"%08X", dwExportRVA);
	//序号基数
	BaseOfOrdinal.Format(L"%08X", pExport->Base);
	//特征值
	FeatureValue.Format(L"%08X", 0);
	//名称
	Name.Format(L"%08X", pExport->Name);
	//函数数量
	NumberOfFuns.Format(L"%04d", pExport->NumberOfFunctions);
	//函数名数量
	NumberOfNamefuns.Format(L"%04d", pExport->NumberOfNames);
	//函数地址表地址
	AddressOfAddressTable.Format(L"%08X", pExport->AddressOfFunctions);
	//函数名称表地址
	AddressOfNameTable.Format(L"%08X", pExport->AddressOfNames);
	//函数序号表地址
	AddressOfOrdinalTable.Format(L"%08X", pExport->AddressOfNameOrdinals);
	//模块名
	ModelName.Format(L"%S", (CTool::RVAtoFOA(pExport->Name, CTool::pFileBuf)) + CTool::pFileBuf);
	UpdateData(FALSE);


	//下部分信息
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	ExportTableList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//设置列表头
	ExportTableList.InsertColumn(0, L"序号", 0, 100);
	ExportTableList.InsertColumn(1, L"RVA", 0, 100);
	ExportTableList.InsertColumn(2, L"FOA", 0, 150);
	ExportTableList.InsertColumn(3, L"函数名", 0, 200);

	//获取三张字表的地址 , 函数地址表 , 名称表 , 序号表的FOA
	DWORD dwFunFOA = CTool::RVAtoFOA(pExport->AddressOfFunctions, CTool::pFileBuf);
	DWORD dwNameFOA = CTool::RVAtoFOA(pExport->AddressOfNames, CTool::pFileBuf);
	DWORD dwOrdinalFOA = CTool::RVAtoFOA(pExport->AddressOfNameOrdinals, CTool::pFileBuf);

	//获取三张表在文件中的实际地址
	DWORD* pFunAddr = (DWORD*)(dwFunFOA + CTool::pFileBuf);
	DWORD* pNameAddr = (DWORD*)(dwNameFOA + CTool::pFileBuf);
	//序号表的元素大小是2个字节
	WORD* pOrdinalAddr = (WORD*)(dwOrdinalFOA + CTool::pFileBuf);



	int nCount = 0;
	//循环放置列表数据
	for (DWORD i = 0; i < pExport->NumberOfFunctions; i++)
	{
		ExportTableList.InsertItem(nCount, L"");
		CString TempValue;

		//序号
		TempValue.Format(L"%04d", pOrdinalAddr[i]);
		//参数1:第几行 参数2:第几列 参数3:值
		ExportTableList.SetItemText(nCount, 0, TempValue);

		//函数地址表中的RVA
		DWORD dwFunRVA = pFunAddr[pOrdinalAddr[i]];
		TempValue.Format(L"%08X", dwFunRVA);
		ExportTableList.SetItemText(nCount, 1, TempValue);

		//对应的FOA
		DWORD dwFunFOA = CTool::RVAtoFOA(dwFunRVA, CTool:: pFileBuf);
		TempValue.Format(L"%08X", dwFunFOA);
		ExportTableList.SetItemText(nCount, 2, TempValue);

		//函数名称
		//从名称表中取出对应的名称地址(RVA)
		DWORD dwNameRVA = pNameAddr[i];
		DWORD dwNameFOA = CTool::RVAtoFOA(dwNameRVA, CTool::pFileBuf);
		//函数名称的起始地址
		char* pName = CTool::pFileBuf + dwNameFOA;
		TempValue.Format(L"%S", pName);
		ExportTableList.SetItemText(nCount, 3, TempValue);
		nCount++;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
