// RelocateTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "RelocateTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CRelocateTable �Ի���

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


// CRelocateTable ��Ϣ�������


BOOL CRelocateTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ô�����
	CString WinTitle("�ض�λ��");
	SetWindowText(WinTitle.GetBuffer());

	//���ñ�ͷ
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	BlockList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	BlockList.InsertColumn(0, L"����", 0, 100);
	BlockList.InsertColumn(1, L"��ʼRVA", 0, 100);
	BlockList.InsertColumn(2, L"����", 0, 100);


	ItemList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ItemList.InsertColumn(0, L"RVA", 0, 100);
	ItemList.InsertColumn(1, L"Offset", 0, 100);
	ItemList.InsertColumn(2, L"Type", 0, 100);
	ItemList.InsertColumn(3, L"��Ҫ�޸ĵ�VA", 0, 150);

	//��λ���ض�λ��ĵ�ַ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD dwRelocFOA = CTool::RVAtoFOA(dwRelocRVA, CTool::pFileBuf);
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CTool::pFileBuf);

	//�ض�λ��ṹ��
	struct TypeOffset
	{
		WORD offset : 12;//���VirtualAddress��ҳ��ƫ��
		WORD type : 4;	//�ض�λ����(3:�ض�λ4���ֽ� , 0:�����ض�λ)
	};


	int nCount = 0;
	CString tempStr;
	//�����ض�λ��
	while (pReloc->VirtualAddress)
	{
		//������ض�λ��ĸ���
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);
		BlockList.InsertItem(nCount, L"");
		//����
		//��ȡ��һ�����α�ĵ�ַ
		IMAGE_SECTION_HEADER* pScnHdr = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(pNt);
		//�ж��������ĸ����� , �������α�
		//�������α��е���Ϣ�����ж�
		for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; ++i) {
			if (pReloc->VirtualAddress >= pScnHdr[i].VirtualAddress/*�ض�λ�����ʼλ�� ���ڵ��� ���ε���ʼλ��*/
				&&pReloc->VirtualAddress <= pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData/*�ض�λ�����ʼλ�� С�ڵ��� �������ļ��еĽ���λ��*/)
			{
				//�ж����ĸ�����
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

		//��ʼRVA
		tempStr.Format(L"%08X", pReloc->VirtualAddress);
		BlockList.SetItemText(nCount, 1, tempStr);

		//����
		tempStr.Format(L"%d", dwCount);
		BlockList.SetItemText(nCount, 2, tempStr);

		nCount++;
		//��һ���ض�λ���ַ = ��ǰ�ض�λ��ʼ��ַ + �ض�λ��Ĵ�С
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);

	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CRelocateTable::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	//��ȡѡ�е���
	POSITION ps;
	int index;
	ps = BlockList.GetFirstSelectedItemPosition();
	//��ȡindex�±�
	index = BlockList.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е�����
	CString SelectRVA = BlockList.GetItemText(index, 1);


	//��λ���ض�λ��ĵ�ַ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	DWORD dwRelocRVA = pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD dwRelocFOA = CTool::RVAtoFOA(dwRelocRVA, CTool::pFileBuf);
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + CTool::pFileBuf);

	//�ض�λ��ṹ��
	struct TypeOffset
	{
		WORD offset : 12;//���VirtualAddress��ҳ��ƫ��
		WORD type : 4;	//�ض�λ����(3:�ض�λ4���ֽ� , 0:�����ض�λ)
	};

	int nCount = 0;
	CString tempStr;
	//�����ض�λ��
	while (pReloc->VirtualAddress)
	{
		//��ȡ��һ���ض�λ��ṹ��ĵ�ַ
		TypeOffset* pOffset = (TypeOffset*)(pReloc + 1);

		//������ض�λ��ĸ���
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);
		

		//�û�ѡ���RVA���ض�λ���е�RVA���бȽ�
		CString RVA;
		RVA.Format(_T("%08x"), pReloc->VirtualAddress);
		if (RVA.Compare(SelectRVA) == 0)
		{
			//������Ӧ���ض�λ��
			for (DWORD i = 0; i < dwCount; ++i) {
				//typeΪ3 , ˵����Ҫ�ض�λ4���ֽ�
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

					//Ҫ�ض�λ��VA
					//��ȡ�ض�λ�������ļ��еľ����ַ
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
