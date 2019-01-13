// ImportTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ImportTable.h"
#include "afxdialogex.h"
#include "Tool.h"

// CImportTable �Ի���

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


// CImportTable ��Ϣ�������


BOOL CImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ô�����
	CString WinTitle("�����");
	SetWindowText(WinTitle.GetBuffer());

	//���ñ�ͷ
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	ImportModel.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportModel.InsertColumn(0, L"DLL����", 0, 100);
	ImportModel.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	ImportModel.InsertColumn(2, L"����ʱ���־", 0, 100);
	ImportModel.InsertColumn(3, L"ForwarderChina", 0, 100);
	ImportModel.InsertColumn(4, L"����", 0, 80);
	ImportModel.InsertColumn(5, L"FirstThunk", 0, 100);

	ImportINT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportINT.InsertColumn(0, L"API����", 0, 100);
	ImportINT.InsertColumn(1, L"���", 0, 80);
	ImportINT.InsertColumn(2, L"Thunkֵ", 0, 100);
	//ImportINT.InsertColumn(3, L"Thunkƫ��", 0, 100);
	//ImportINT.InsertColumn(4, L"ThunkRVA", 0, 100);

	ImportIAT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ImportIAT.InsertColumn(0, L"API����", 0, 100);
	ImportIAT.InsertColumn(1, L"���", 0, 80);
	ImportIAT.InsertColumn(2, L"Thunkֵ", 0, 100);
	//ImportIAT.InsertColumn(3, L"Thunkƫ��", 0, 100);
	//ImportIAT.InsertColumn(4, L"ThunkRVA", 0, 100);


	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);


	//������RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//��RVAת��FOA
	DWORD dwImportFOA = CTool::RVAtoFOA(dwImportRVA, CTool::pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + CTool::pFileBuf);

	int nCount = 0;
	//������������
	while (pImport->Name)
	{
		//����ģ�����Ƶ��׵�ַ
		char*  pDllName = CTool::RVAtoFOA(pImport->Name, CTool::pFileBuf) + CTool::pFileBuf;
		CString temp(pDllName);

		ImportModel.InsertItem(nCount, L"");

		//DLL������
		ImportModel.SetItemText(nCount, 0, temp);
		//OriginalFirstThunk , INT
		DWORD dwINT = pImport->OriginalFirstThunk;
		temp.Format(L"%08X", dwINT);
		ImportModel.SetItemText(nCount, 1, temp);


		//����ʱ���־
		temp.Format(L"%08X", 0);
		ImportModel.SetItemText(nCount, 2, temp);


		//ForwarderChina
		temp.Format(L"%08X", pImport->ForwarderChain);
		ImportModel.SetItemText(nCount, 3, temp);


		//����
		temp.Format(L"%08X", pImport->Name);
		ImportModel.SetItemText(nCount, 4, temp);


		//FirstThunk
		temp.Format(L"%08X", pImport->FirstThunk);
		ImportModel.SetItemText(nCount, 5, temp);

		//��һ�������
		++pImport;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CImportTable::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CImportTable::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��ȡѡ�е���
	POSITION ps;
	int index;
	ps = ImportModel.GetFirstSelectedItemPosition();
	//��ȡindex�±�
	index = ImportModel.GetNextSelectedItem(ps);
	//��ȡѡ������ָ���е�����
	CString SelectedDllName = ImportModel.GetItemText(index, 0);

	//��ȡPEͷ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//������RVA
	DWORD dwImportRVA = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//��RVAת��FOA
	DWORD dwImportFOA = CTool::RVAtoFOA(dwImportRVA, CTool::pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + CTool::pFileBuf);


	CString tempStr;
	//����ÿ��ģ��
	while (pImport->Name != 0)
	{
		char*  pDllName = CTool::RVAtoFOA(pImport->Name, CTool::pFileBuf) + CTool::pFileBuf;
		CString DllName(pDllName);

		//�û�ѡ�е�ģ���ÿ��ģ���������Ƚ�
		//����0��ʾ���
		if (SelectedDllName.Compare(DllName) != 0){
			//������0 , ����� , ������һ��ģ��
			++pImport;
			continue;
		}

		//�ҵ���ָ����ģ��
		//��ȡINT�ĵ�ַ
		PIMAGE_THUNK_DATA pINTAddr = (PIMAGE_THUNK_DATA)
			(CTool::RVAtoFOA(pImport->OriginalFirstThunk, CTool::pFileBuf) + CTool::pFileBuf);

		int count = 0;
		
		//����IAT��
		while (pINTAddr->u1.AddressOfData)
		{
			//�жϺ����ĵ��뷽ʽ
			//������λΪ1 , ˵������ŵ��� , ����������Ƶ���
			//�ж����λ��ֵ
			if (pINTAddr->u1.AddressOfData & 0x80000000)
			{
				//��ŵ��� , ��ô���λ2���ֽھ����亯�����
				//printf(" -->name:[NULL]  ordinal:[%04X]\n",pINTAddr->u1.AddressOfData & 0xFFFF);
				
				ImportINT.InsertItem(count, L"");

				//API����

				//���
				//Thunkֵ
				//Thunkƫ��
				//ThunkRVA
			}
			else
			{
				ImportINT.InsertItem(count, L"");
				ImportIAT.InsertItem(count , L"");

				//���Ƶ���
				DWORD dwNameRVA = pINTAddr->u1.AddressOfData;
				DWORD dwNameFOA = CTool::RVAtoFOA(dwNameRVA, CTool::pFileBuf);
				PIMAGE_IMPORT_BY_NAME pName =
					(PIMAGE_IMPORT_BY_NAME)
					(dwNameFOA + CTool::pFileBuf);
				//printf(" -->name:[%s]  ordinal:[%04X]\n",pName->Name, pName->Hint);

				//API����
				tempStr.Format(L"%S", pName->Name);
				ImportINT.SetItemText(count, 0, tempStr);
				ImportIAT.SetItemText(count, 0, tempStr);
				//���
				tempStr.Format(L"%04d", pName->Hint);
				ImportINT.SetItemText(count, 1, tempStr);
				ImportIAT.SetItemText(count, 1, tempStr);
				//Thunkֵ
				tempStr.Format(L"%08X", pINTAddr->u1.Function);
				ImportINT.SetItemText(count, 2, tempStr);
				ImportIAT.SetItemText(count, 2, tempStr);

				//Thunkƫ��
				//ImportINT.SetItemText(count, 3, tempStr);
				//ThunkRVA
				//ImportINT.SetItemText(count, 4, tempStr);
			}

			count++;
			pINTAddr++;
		}

		break;

	}





	*pResult = 0;
}
