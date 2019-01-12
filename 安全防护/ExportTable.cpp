// ExportTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ExportTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CExportTable �Ի���

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


// CExportTable ��Ϣ�������


BOOL CExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//���öԻ�����
	CString DlgTitle("������");
	SetWindowText(DlgTitle.GetBuffer());

	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//��ȡ�������RVA
	DWORD dwExportRVA = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//RVAת��ΪFOA
	DWORD dwExportFOA = CTool::RVAtoFOA(dwExportRVA, CTool::pFileBuf);
	//���������
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(CTool::pFileBuf + dwExportFOA);

	//�ϲ�����Ϣ
	//�����RVA
	ExportTableRVA.Format(L"%08X", dwExportRVA);
	//��Ż���
	BaseOfOrdinal.Format(L"%08X", pExport->Base);
	//����ֵ
	FeatureValue.Format(L"%08X", 0);
	//����
	Name.Format(L"%08X", pExport->Name);
	//��������
	NumberOfFuns.Format(L"%04d", pExport->NumberOfFunctions);
	//����������
	NumberOfNamefuns.Format(L"%04d", pExport->NumberOfNames);
	//������ַ���ַ
	AddressOfAddressTable.Format(L"%08X", pExport->AddressOfFunctions);
	//�������Ʊ��ַ
	AddressOfNameTable.Format(L"%08X", pExport->AddressOfNames);
	//������ű��ַ
	AddressOfOrdinalTable.Format(L"%08X", pExport->AddressOfNameOrdinals);
	//ģ����
	ModelName.Format(L"%S", (CTool::RVAtoFOA(pExport->Name, CTool::pFileBuf)) + CTool::pFileBuf);
	UpdateData(FALSE);


	//�²�����Ϣ
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	ExportTableList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//�����б�ͷ
	ExportTableList.InsertColumn(0, L"���", 0, 100);
	ExportTableList.InsertColumn(1, L"RVA", 0, 100);
	ExportTableList.InsertColumn(2, L"FOA", 0, 150);
	ExportTableList.InsertColumn(3, L"������", 0, 200);

	//��ȡ�����ֱ�ĵ�ַ , ������ַ�� , ���Ʊ� , ��ű��FOA
	DWORD dwFunFOA = CTool::RVAtoFOA(pExport->AddressOfFunctions, CTool::pFileBuf);
	DWORD dwNameFOA = CTool::RVAtoFOA(pExport->AddressOfNames, CTool::pFileBuf);
	DWORD dwOrdinalFOA = CTool::RVAtoFOA(pExport->AddressOfNameOrdinals, CTool::pFileBuf);

	//��ȡ���ű����ļ��е�ʵ�ʵ�ַ
	DWORD* pFunAddr = (DWORD*)(dwFunFOA + CTool::pFileBuf);
	DWORD* pNameAddr = (DWORD*)(dwNameFOA + CTool::pFileBuf);
	//��ű��Ԫ�ش�С��2���ֽ�
	WORD* pOrdinalAddr = (WORD*)(dwOrdinalFOA + CTool::pFileBuf);



	int nCount = 0;
	//ѭ�������б�����
	for (DWORD i = 0; i < pExport->NumberOfFunctions; i++)
	{
		ExportTableList.InsertItem(nCount, L"");
		CString TempValue;

		//���
		TempValue.Format(L"%04d", pOrdinalAddr[i]);
		//����1:�ڼ��� ����2:�ڼ��� ����3:ֵ
		ExportTableList.SetItemText(nCount, 0, TempValue);

		//������ַ���е�RVA
		DWORD dwFunRVA = pFunAddr[pOrdinalAddr[i]];
		TempValue.Format(L"%08X", dwFunRVA);
		ExportTableList.SetItemText(nCount, 1, TempValue);

		//��Ӧ��FOA
		DWORD dwFunFOA = CTool::RVAtoFOA(dwFunRVA, CTool:: pFileBuf);
		TempValue.Format(L"%08X", dwFunFOA);
		ExportTableList.SetItemText(nCount, 2, TempValue);

		//��������
		//�����Ʊ���ȡ����Ӧ�����Ƶ�ַ(RVA)
		DWORD dwNameRVA = pNameAddr[i];
		DWORD dwNameFOA = CTool::RVAtoFOA(dwNameRVA, CTool::pFileBuf);
		//�������Ƶ���ʼ��ַ
		char* pName = CTool::pFileBuf + dwNameFOA;
		TempValue.Format(L"%S", pName);
		ExportTableList.SetItemText(nCount, 3, TempValue);
		nCount++;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
