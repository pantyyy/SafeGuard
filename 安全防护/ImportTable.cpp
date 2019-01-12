// ImportTable.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "ImportTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CImportTable 对话框

IMPLEMENT_DYNAMIC(CImportTable, CDialogEx)

CImportTable::CImportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportTable::IDD, pParent)
{

}

CImportTable::~CImportTable()
{
}

void CImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, ImportModel);
	DDX_Control(pDX, IDC_LIST4, ImportINT);
	DDX_Control(pDX, IDC_LIST1, ImportIAT);
}


BEGIN_MESSAGE_MAP(CImportTable, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CImportTable::OnLvnItemchangedList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CImportTable::OnNMClickList3)
END_MESSAGE_MAP()


// CImportTable 消息处理程序


BOOL CImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置窗口名
	CString WinTitle("导入表");
	SetWindowText(WinTitle.GetBuffer());

	//设置表头
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	ImportModel.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportModel.InsertColumn(0, L"DLL名称", 0, 100);
	ImportModel.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	ImportModel.InsertColumn(2, L"日期时间标志", 0, 100);
	ImportModel.InsertColumn(3, L"ForwarderChina", 0, 100);
	ImportModel.InsertColumn(4, L"名称", 0, 80);
	ImportModel.InsertColumn(5, L"FirstThunk", 0, 100);

	ImportINT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportINT.InsertColumn(0, L"API名称", 0, 100);
	ImportINT.InsertColumn(1, L"序号", 0, 80);
	ImportINT.InsertColumn(2, L"Thunk值", 0, 100);
	ImportINT.InsertColumn(3, L"Thunk偏移", 0, 100);
	ImportINT.InsertColumn(4, L"ThunkRVA", 0, 100);

	ImportIAT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportIAT.InsertColumn(0, L"API名称", 0, 100);
	ImportIAT.InsertColumn(1, L"序号", 0, 80);
	ImportIAT.InsertColumn(2, L"Thunk值", 0, 100);
	ImportIAT.InsertColumn(3, L"Thunk偏移", 0, 100);
	ImportIAT.InsertColumn(4, L"ThunkRVA", 0, 100);


	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);


	//导入表的RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//将RVA转成FOA
	DWORD dwImportFOA = CTool::RVAtoFOA(dwImportRVA, CTool::pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + CTool::pFileBuf);

	int nCount = 0;
	//遍历多个导入表
	while (pImport->Name)
	{
		//导入模块名称的首地址
		char*  pDllName = CTool::RVAtoFOA(pImport->Name, CTool::pFileBuf) + CTool::pFileBuf;
		CString temp(pDllName);

		ImportModel.InsertItem(nCount, L"");

		//DLL的名称
		ImportModel.SetItemText(nCount, 0, temp);
		//OriginalFirstThunk , INT
		DWORD dwINT = pImport->OriginalFirstThunk;
		temp.Format(L"%08X", dwINT);
		ImportModel.SetItemText(nCount, 1, temp);


		//日期时间标志
		temp.Format(L"%08X", 0);
		ImportModel.SetItemText(nCount, 2, temp);


		//ForwarderChina
		temp.Format(L"%08X", pImport->ForwarderChain);
		ImportModel.SetItemText(nCount, 3, temp);


		//名称
		temp.Format(L"%08X", pImport->Name);
		ImportModel.SetItemText(nCount, 4, temp);


		//FirstThunk
		temp.Format(L"%08X", pImport->FirstThunk);
		ImportModel.SetItemText(nCount, 5, temp);

		//下一个导入表
		++pImport;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CImportTable::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CImportTable::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	//获取选中的行
	POSITION ps;
	int index;
	ps = ImportModel.GetFirstSelectedItemPosition();
	//获取index下标
	index = ImportModel.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据
	CString DllName = ImportModel.GetItemText(index, 0);

	//获取PE头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//导入表的RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//将RVA转成FOA
	DWORD dwImportFOA = CTool::RVAtoFOA(dwImportRVA, CTool::pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + CTool::pFileBuf);







	*pResult = 0;
}
