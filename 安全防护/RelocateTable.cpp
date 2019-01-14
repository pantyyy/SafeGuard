// RelocateTable.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "RelocateTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CRelocateTable 对话框

IMPLEMENT_DYNAMIC(CRelocateTable, CDialogEx)

CRelocateTable::CRelocateTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRelocateTable::IDD, pParent)
{

}

CRelocateTable::~CRelocateTable()
{
}

void CRelocateTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, BlockList);
	DDX_Control(pDX, IDC_LIST2, ItemList);
}


BEGIN_MESSAGE_MAP(CRelocateTable, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CRelocateTable::OnNMClickList1)
END_MESSAGE_MAP()


// CRelocateTable 消息处理程序


BOOL CRelocateTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置窗口名
	CString WinTitle("重定位表");
	SetWindowText(WinTitle.GetBuffer());

	//设置表头
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	BlockList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	BlockList.InsertColumn(0, L"区段", 0, 100);
	BlockList.InsertColumn(1, L"起始RVA", 0, 100);
	BlockList.InsertColumn(2, L"个数", 0, 100);


	ItemList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ItemList.InsertColumn(0, L"RVA", 0, 100);
	ItemList.InsertColumn(1, L"Offset", 0, 100);
	ItemList.InsertColumn(2, L"Type", 0, 100);
	ItemList.InsertColumn(3, L"需要修改的VA", 0, 150);

	//定位到重定位表的地址
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD dwRelocFOA = CTool::RVAtoFOA(dwRelocRVA, CTool::pFileBuf);
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CTool::pFileBuf);

	//重定位项结构体
	struct TypeOffset
	{
		WORD offset : 12;//相对VirtualAddress的页内偏移
		WORD type : 4;	//重定位类型(3:重定位4个字节 , 0:无需重定位)
	};


	int nCount = 0;
	CString tempStr;
	//遍历重定位块
	while (pReloc->VirtualAddress)
	{
		//计算出重定位项的个数
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);
		BlockList.InsertItem(nCount, L"");
		//区段
		//获取第一个区段表的地址
		IMAGE_SECTION_HEADER* pScnHdr = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(pNt);
		//判断是属于哪个区段 , 遍历区段表
		//根据区段表中的信息进行判断
		for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; ++i) {
			if (pReloc->VirtualAddress >= pScnHdr[i].VirtualAddress/*重定位块的起始位置 大于等于 区段的起始位置*/
				&&pReloc->VirtualAddress <= pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData/*重定位块的起始位置 小于等于 区段在文件中的结束位置*/)
			{
				//判断是哪个区段
				switch (i)
				{
				case 1:
					tempStr = L"(.text)";
					break;
				case 2:
					tempStr = L"(.rdata)";
					break;
				case 3:
					tempStr = L"(.data)";
					break;
				default:
					tempStr = L"(.text)";
					break;
				}
			}
		}
		BlockList.SetItemText(nCount, 0, tempStr);

		//起始RVA
		tempStr.Format(L"%08X", pReloc->VirtualAddress);
		BlockList.SetItemText(nCount, 1, tempStr);

		//个数
		tempStr.Format(L"%d", dwCount);
		BlockList.SetItemText(nCount, 2, tempStr);

		nCount++;
		//下一个重定位块地址 = 当前重定位起始地址 + 重定位块的大小
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);

	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CRelocateTable::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	//获取选中的行
	POSITION ps;
	int index;
	ps = BlockList.GetFirstSelectedItemPosition();
	//获取index下标
	index = BlockList.GetNextSelectedItem(ps);
	//获取选中行中指定列的数据
	CString SelectRVA = BlockList.GetItemText(index, 1);


	//定位到重定位表的地址
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD dwRelocFOA = CTool::RVAtoFOA(dwRelocRVA, CTool::pFileBuf);
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CTool::pFileBuf);

	//重定位项结构体
	struct TypeOffset
	{
		WORD offset : 12;//相对VirtualAddress的页内偏移
		WORD type : 4;	//重定位类型(3:重定位4个字节 , 0:无需重定位)
	};

	int nCount = 0;
	CString tempStr;
	//遍历重定位块
	while (pReloc->VirtualAddress)
	{
		//获取第一个重定位项结构体的地址
		TypeOffset* pOffset = (TypeOffset*)(pReloc + 1);

		//计算出重定位项的个数
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);
		

		//用户选择的RVA和重定位块中的RVA进行比较
		CString RVA;
		RVA.Format(_T("%08x"), pReloc->VirtualAddress);
		if (RVA.Compare(SelectRVA) == 0)
		{
			//遍历对应的重定位项
			for (DWORD i = 0; i < dwCount; ++i) {
				//type为3 , 说明需要重定位4个字节
				if (pOffset->type == 3)
				{
					ItemList.InsertItem(nCount, L"");

					//RVA
					tempStr.Format(L"%08X", pOffset[i].offset + pReloc->VirtualAddress);
					ItemList.SetItemText(nCount, 0 , tempStr);

					//Offset
					tempStr.Format(L"%08X", CTool::RVAtoFOA(pOffset[i].offset + pReloc->VirtualAddress, CTool::pFileBuf));
					ItemList.SetItemText(nCount, 1 , tempStr);

					//type
					ItemList.SetItemText(nCount, 2 , L"HIGHLOW(3)");

					//要重定位的VA
					//获取重定位数据在文件中的具体地址
					DWORD* pRelocDataAddr = (DWORD*)(CTool::RVAtoFOA(pOffset[i].offset + pReloc->VirtualAddress, CTool::pFileBuf) + CTool::pFileBuf);
					tempStr.Format(L"%08X", *pRelocDataAddr);
					ItemList.SetItemText(nCount, 3 , tempStr);

					nCount++;
				}

			}

			break;
		}

	}




}
