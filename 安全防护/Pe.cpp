// Pe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
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
// CPe �Ի���

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


// CPe ��Ϣ�������


void CPe::OnBnClickedButtonSelectpath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);
	dlg.DoModal();

	//��ȡ�ļ�·��
	CString path = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
	FilePath = path;
	//���ݸ��µ��ؼ���
	UpdateData(FALSE);
}


void CPe::OnBnClickedButtonResolve()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CTool::pFileBuf = CTool::GetFileMemPointer(FilePath);

	if (CTool::pFileBuf == NULL)
	{
		MessageBox(L"���ļ�ʧ��! \n");
		return;
	}

	//�ж��Ƿ���PE�ļ�
	if (CTool::IsPeFile(CTool::pFileBuf))
	{
		//MessageBox(L"PE�ļ���Ч��\n");
		//������Ϣ

		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
		PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
		IMAGE_OPTIONAL_HEADER32 *pOt = &pNt->OptionalHeader;

		//������ڵ�
		AddressOfEntryPoint.Format(L"%08X", pOt->AddressOfEntryPoint);
		//�����ַ = �����ڴ���ػ�ַ
		ImageBase.Format(L"%08X", pOt->ImageBase);
		//�����С = PE���ڴ��ж���֮��Ĵ�С
		SizeOfImage.Format(L"%08X", pOt->SizeOfImage);
		//�����ַ
		BaseOfCode.Format(L"%08X", pOt->BaseOfCode);
		//���ݻ�ַ
		BaseOfData.Format(L"%08X", pOt->BaseOfData);
		//�ڴ����
		SectionAligment.Format(L"%08X", pOt->SectionAlignment);
		//�ļ�����
		FileAlignment.Format(L"%08X", pOt->FileAlignment);
		//��־��
		Magic.Format(L"%08X", pOt->Magic);


		//��ϵͳ
		Subsystem.Format(L"%08X", pOt->Subsystem);
		//������Ŀ
		NumberOfSections.Format(L"%08X", pNt->FileHeader.NumberOfSections);
		//����ʱ��
		TimeDataStamp.Format(L"%08X", pNt->FileHeader.TimeDateStamp);
		//ͷ����С
		SizeOfHeaders.Format(L"%08X", pOt->SizeOfHeaders);
		//����ֵ
		DllCharacteristics.Format(L"%08X", pOt->DllCharacteristics);
		//У��ֵ
		CheckSum.Format(L"%08X", pOt->CheckSum);
		//��ѡͷ����С , ����չPEͷ��С
		SizeOfOptionalHeader.Format(L"%08X", pNt->FileHeader.SizeOfOptionalHeader);
		//RVA������С
		NumberOfRvaAndSizes.Format(L"%08X", pOt->NumberOfRvaAndSizes);

		UpdateData(FALSE);
		return;
	}
	else
	{
		MessageBox(L"��Ч��PE�ļ���\n");
		return;
	}


}


void CPe::OnBnClickedButtonExport()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//��ȡ�������RVA
	DWORD dwExportRVA = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	if (dwExportRVA == 0)
	{
		MessageBox(L"û�е�����!");
		return;
	}

	CExportTable* ExportTable = new CExportTable();
	ExportTable->Create(IDD_DIALOG_ExportTable);
	ExportTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonResourcetable()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CResourceTable* ResourceTable = new CResourceTable();
	ResourceTable->Create(IDD_DIALOG_Resource);
	ResourceTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonImport()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//������RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	if (dwImportRVA == 0)
	{
		MessageBox(L"û�е����");
		return;
	}

	CImportTable* ImportTable = new CImportTable();
	ImportTable->Create(IDD_DIALOG_ImportTable);
	ImportTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonRelocate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��λ���ض�λ��ĵ�ַ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;

	if (dwRelocRVA == 0)
	{
		MessageBox(L"û���ض�λ��");
		return;
	}

	CRelocateTable* RelocateTable = new CRelocateTable();
	RelocateTable->Create(IDD_DIALOG_RelocateTable);
	RelocateTable->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonSection()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSectionInfo* SectionInfo = new CSectionInfo();
	SectionInfo->Create(IDD_DIALOG_SectionInfo);
	SectionInfo->ShowWindow(SW_SHOW);

}


void CPe::OnBnClickedButtonDir()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDirInfo* DirInfo = new CDirInfo();
	DirInfo->Create(IDD_DIALOG_Dir);
	DirInfo->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonDelay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDelay* Delay = new CDelay();
	Delay->Create(IDD_DIALOG_Delay);
	Delay->ShowWindow(SW_SHOW);
}


void CPe::OnBnClickedButtonTls()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CTls* Tls = new CTls();
	Tls->Create(IDD_DIALOG_TLS);
	Tls->ShowWindow(SW_SHOW);
}
