// Pe.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Pe.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "ExportTable.h"
#include "ResourceTable.h"
#include "ImportTable.h"
#include "RelocateTable.h"
#include "SectionInfo.h"
#include "DirInfo.h"
#include "Tls.h"
#include "Delay.h"
// CPe 对话框

IMPLEMENT_DYNAMIC(CPe, CDialogEx)

CPe::CPe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPe::IDD, pParent)
	, FilePath(_T(""))
	, AddressOfEntryPoint(_T(""))
	, ImageBase(_T(""))
	, SizeOfImage(_T(""))
	, BaseOfCode(_T(""))
	, BaseOfData(_T(""))
	, SectionAligment(_T(""))
	, FileAlignment(_T(""))
	, Magic(_T(""))
	, Subsystem(_T(""))
	, NumberOfSections(_T(""))
	, TimeDataStamp(_T(""))
	, SizeOfHeaders(_T(""))
	, DllCharacteristics(_T(""))
	, CheckSum(_T(""))
	, SizeOfOptionalHeader(_T(""))
	, NumberOfRvaAndSizes(_T(""))
{

}

CPe::~CPe()
{
}

void CPe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FilePath, FilePath);
	DDX_Text(pDX, IDC_EDIT2, AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT3, ImageBase);
	DDX_Text(pDX, IDC_EDIT4, SizeOfImage);
	DDX_Text(pDX, IDC_EDIT5, BaseOfCode);
	DDX_Text(pDX, IDC_EDIT6, BaseOfData);
	DDX_Text(pDX, IDC_EDIT7, SectionAligment);
	DDX_Text(pDX, IDC_EDIT8, FileAlignment);
	DDX_Text(pDX, IDC_EDIT9, Magic);
	DDX_Text(pDX, IDC_EDIT10, Subsystem);
	DDX_Text(pDX, IDC_EDIT11, NumberOfSections);
	DDX_Text(pDX, IDC_EDIT12, TimeDataStamp);
	DDX_Text(pDX, IDC_EDIT13, SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT14, DllCharacteristics);
	DDX_Text(pDX, IDC_EDIT15, CheckSum);
	DDX_Text(pDX, IDC_EDIT16, SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT17, NumberOfRvaAndSizes);
}


BEGIN_MESSAGE_MAP(CPe, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SelectPath, &CPe::OnBnClickedButtonSelectpath)
	ON_BN_CLICKED(IDC_BUTTON_Resolve, &CPe::OnBnClickedButtonResolve)
	ON_BN_CLICKED(IDC_BUTTON_Export, &CPe::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_ResourceTable, &CPe::OnBnClickedButtonResourcetable)
	ON_BN_CLICKED(IDC_BUTTON_Import, &CPe::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_Relocate, &CPe::OnBnClickedButtonRelocate)
	ON_BN_CLICKED(IDC_BUTTON_Section, &CPe::OnBnClickedButtonSection)
	ON_BN_CLICKED(IDC_BUTTON_Dir, &CPe::OnBnClickedButtonDir)
	ON_BN_CLICKED(IDC_BUTTON_Delay, &CPe::OnBnClickedButtonDelay)
	ON_BN_CLICKED(IDC_BUTTON_TLS, &CPe::OnBnClickedButtonTls)
END_MESSAGE_MAP()


// CPe 消息处理程序


void CPe::OnBnClickedButtonSelectpath()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	dlg.DoModal();

	//获取文件路径
	CString path = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
	FilePath = path;
	//数据更新到控件上
	UpdateData(FALSE);
}


void CPe::OnBnClickedButtonResolve()
{
	// TODO:  在此添加控件通知处理程序代码
	CTool::pFileBuf = CTool::GetFileMemPointer(FilePath);

	if (CTool::pFileBuf == NULL)
	{
		MessageBox(L"打开文件失败! \n");
		return;
	}

	//判断是否是PE文件
	if (CTool::IsPeFile(CTool::pFileBuf))
	{
		//MessageBox(L"PE文件有效！\n");
		//解析信息

		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
		PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
		IMAGE_OPTIONAL_HEADER32 *pOt = &pNt->OptionalHeader;

		//程序入口点
		AddressOfEntryPoint.Format(L"%08X", pOt->AddressOfEntryPoint);
		//镜像基址 = 虚拟内存加载基址
		ImageBase.Format(L"%08X", pOt->ImageBase);
		//镜像大小 = PE在内存中对齐之后的大小
		SizeOfImage.Format(L"%08X", pOt->SizeOfImage);
		//代码基址
		BaseOfCode.Format(L"%08X", pOt->BaseOfCode);
		//数据基址
		BaseOfData.Format(L"%08X", pOt->BaseOfData);
		//内存对齐
		SectionAligment.Format(L"%08X", pOt->SectionAlignment);
		//文件对齐
		FileAlignment.Format(L"%08X", pOt->FileAlignment);
		//标志字
		Magic.Format(L"%08X", pOt->Magic);


		//子系统
		Subsystem.Format(L"%08X", pOt->Subsystem);
		//区段数目
		NumberOfSections.Format(L"%08X", pNt->FileHeader.NumberOfSections);
		//日期时间
		TimeDataStamp.Format(L"%08X", pNt->FileHeader.TimeDateStamp);
		//头部大小
		SizeOfHeaders.Format(L"%08X", pOt->SizeOfHeaders);
		//特征值
		DllCharacteristics.Format(L"%08X", pOt->DllCharacteristics);
		//校验值
		CheckSum.Format(L"%08X", pOt->CheckSum);
		//可选头部大小 , 即扩展PE头大小
		SizeOfOptionalHeader.Format(L"%08X", pNt->FileHeader.SizeOfOptionalHeader);
		//RVA数及大小
		NumberOfRvaAndSizes.Format(L"%08X", pOt->NumberOfRvaAndSizes);

		UpdateData(FALSE);
		return;
	}
	else
	{
		MessageBox(L"无效的PE文件！\n");
		return;
	}


}


void CPe::OnBnClickedButtonExport()
{
	// TODO:  在此添加控件通知处理程序代码

	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//获取导出表的RVA
	DWORD dwExportRVA = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	if (dwExportRVA == 0)
	{
		MessageBox(L"没有导出表!");
		return;
	}

	CExportTable* ExportTable = new CExportTable();
	ExportTable->Create(IDD_DIALOG_ExportTable);
	ExportTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonResourcetable()
{
	// TODO:  在此添加控件通知处理程序代码
	CResourceTable* ResourceTable = new CResourceTable();
	ResourceTable->Create(IDD_DIALOG_Resource);
	ResourceTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonImport()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//导入表的RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	if (dwImportRVA == 0)
	{
		MessageBox(L"没有导入表");
		return;
	}

	CImportTable* ImportTable = new CImportTable();
	ImportTable->Create(IDD_DIALOG_ImportTable);
	ImportTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonRelocate()
{
	// TODO:  在此添加控件通知处理程序代码

	//定位到重定位表的地址
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;

	if (dwRelocRVA == 0)
	{
		MessageBox(L"没有重定位表");
		return;
	}

	CRelocateTable* RelocateTable = new CRelocateTable();
	RelocateTable->Create(IDD_DIALOG_RelocateTable);
	RelocateTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonSection()
{
	// TODO:  在此添加控件通知处理程序代码
	CSectionInfo* SectionInfo = new CSectionInfo();
	SectionInfo->Create(IDD_DIALOG_SectionInfo);
	SectionInfo->ShowWindow(SW_SHOW);

}


void CPe::OnBnClickedButtonDir()
{
	// TODO:  在此添加控件通知处理程序代码
	CDirInfo* DirInfo = new CDirInfo();
	DirInfo->Create(IDD_DIALOG_Dir);
	DirInfo->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonDelay()
{
	// TODO:  在此添加控件通知处理程序代码
	CDelay* Delay = new CDelay();
	Delay->Create(IDD_DIALOG_Delay);
	Delay->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonTls()
{
	// TODO:  在此添加控件通知处理程序代码
	CTls* Tls = new CTls();
	Tls->Create(IDD_DIALOG_TLS);
	Tls->ShowWindow(SW_SHOW);
}
