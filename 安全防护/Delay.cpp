// Delay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "Delay.h"
#include "afxdialogex.h"
#include "Tool.h"

// CDelay �Ի���

IMPLEMENT_DYNAMIC(CDelay, CDialogEx)

CDelay::CDelay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDelay::IDD, pParent)
{

}

CDelay::~CDelay()
{
}

void CDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_delay_1);
	DDX_Control(pDX, IDC_LIST2, m_delay_2);
}


BEGIN_MESSAGE_MAP(CDelay, CDialogEx)
END_MESSAGE_MAP()


// CDelay ��Ϣ�������


BOOL CDelay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString WinTitle("�ӳټ��ر�");
	SetWindowText(WinTitle);

	m_delay_1.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_delay_1.InsertColumn(0, L"Dll����", 0, 100);
	m_delay_1.InsertColumn(1, L"IAT RVA", 0, 50);
	m_delay_1.InsertColumn(2, L"INA RVA", 0, 50);
	m_delay_1.InsertColumn(3, L"��IAT RVA", 0, 200);
	m_delay_1.InsertColumn(4, L"ж�غ��� RVA", 0, 200);
	//��ʼ��������б�
	m_delay_2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_delay_2.InsertColumn(0, L"���", 0, 50);
	m_delay_2.InsertColumn(1, L"����", 0, 300);
	m_delay_2.InsertColumn(2, L"ƫ�Ƶ�ַ", 0, 100);

	//��ȡPEͷ����
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//��һ��: ��ͨ����չͷ�ҵ�����Ŀ¼�ĵ�1��, ��ȡ�������׵�ַ.
	DWORD dwImpTabRva = pNt->OptionalHeader.DataDirectory[13].VirtualAddress;

	//1.1 ��Ϊ�ļ�û���ڴ�������չ��, ���Բ���ֱ��ʹ��rva
	//    ��λ�ļ��е�����, ���,��Ҫ��rvaת�����ļ�ƫ��ȥ��λ.
	DWORD dwImpTabFoa = CTool::RVAtoFOA(dwImpTabRva, CTool::pFileBuf);
	// �õ�����һ�������������׵�ַ
	_IMAGE_DELAYLOAD_DESCRIPTOR* pImp =
		(_IMAGE_DELAYLOAD_DESCRIPTOR*)(dwImpTabFoa + CTool::pFileBuf);
	// 	if (pImp==NULL)
	// 	{
	// 		MessageBox(L"û���ӳټ���");
	// 	}
	nItem = -1;
	int  i = 0;
	CString temp;
	//�ڶ��� : �����������������, ��������һ��ȫ��Ԫ����Ϊ��β.
	while (pImp->DllNameRVA != 0)
	{
		//������ : �ڱ�����ÿһ�������ṹ��ʱ, ��������INT��IAT.
		// 3.1 ��ȡ��������dll������
		DWORD dwNameFoa = CTool::RVAtoFOA(pImp->DllNameRVA, CTool::pFileBuf); // NameҲ��rva,��Ҫת��Ϊfoa
		char* pDllName = (char*)(dwNameFoa + CTool::pFileBuf);
		//ת����WideChar
		int num = MultiByteToWideChar(0, 0, pDllName, -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(0, 0, pDllName, -1, wide, num);

		//����dll����
		m_delay_1.InsertItem(i, NULL);
		m_delay_1.SetItemText(i, 0, wide);

		// 3.2 ��INT/IAT�еõ����еĵ��뺯��������/���
		IMAGE_THUNK_DATA* pIat = 0, *pInt = 0;
		DWORD dwIntFoa = CTool::RVAtoFOA(pImp->ImportNameTableRVA, CTool::pFileBuf); // INT�����rva
		pInt = (IMAGE_THUNK_DATA*)(dwIntFoa + CTool::pFileBuf);
		temp.Format(L"%x", pImp->ImportNameTableRVA);
		m_delay_1.SetItemText(i, 1, temp);

		DWORD dwIatFoa = CTool::RVAtoFOA(pImp->ImportAddressTableRVA, CTool::pFileBuf); // INT�����rva
		pIat = (IMAGE_THUNK_DATA*)(dwIatFoa + CTool::pFileBuf);
		temp.Format(L"%x", pImp->ImportAddressTableRVA);
		m_delay_1.SetItemText(i, 2, temp);

		temp.Format(L"%x", pImp->BoundImportAddressTableRVA);
		m_delay_1.SetItemText(i, 3, temp);

		temp.Format(L"%x", pImp->UnloadInformationTableRVA);
		m_delay_1.SetItemText(i, 4, temp);

		++i;
		++pImp; // �ҵ���һ�������Ԫ��.
		nItem++;
	}
	//����������������������������������������������������������������������������������������������������
	if (nItem != -1)
	{
		//��һ��: ��ͨ����չͷ�ҵ�����Ŀ¼�ĵ�1��, ��ȡ�������׵�ַ.
		DWORD dwImpTabRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;

		//1.1 ��Ϊ�ļ�û���ڴ�������չ��, ���Բ���ֱ��ʹ��rva
		//    ��λ�ļ��е�����, ���,��Ҫ��rvaת�����ļ�ƫ��ȥ��λ.
		DWORD dwImpTabFoa = CTool::RVAtoFOA(dwImpTabRva, CTool::pFileBuf);
		// �õ�����һ�������������׵�ַ
		_IMAGE_DELAYLOAD_DESCRIPTOR* pImp =
			(_IMAGE_DELAYLOAD_DESCRIPTOR*)(dwImpTabFoa + CTool::pFileBuf);
		int  i = 0;
		CString temp;
		//��ȡ��ַ
		pImp += nItem;

		// 3.2 ��INT/IAT�еõ����еĵ��뺯��������/���
		IMAGE_THUNK_DATA32* pIat = 0, *pInt = 0;
		DWORD dwIntFoa = CTool::RVAtoFOA(pImp->ImportNameTableRVA, CTool::pFileBuf); // INT�����rva
		pInt = (IMAGE_THUNK_DATA32*)(dwIntFoa + CTool::pFileBuf);

		DWORD dwIatFoa = CTool::RVAtoFOA(pImp->ImportAddressTableRVA, CTool::pFileBuf); // INT�����rva
		pIat = (IMAGE_THUNK_DATA32*)(dwIatFoa + CTool::pFileBuf);
		//ʹ������һ����Ϳ��Եõ����뺯������Ϣ��
		//�õ���INT��һ��IMAGE_THUNK_DATA������,��
		//ȫ0Ԫ����Ϊ��β.
		while (pInt->u1.Function != 0)
		{
			// �жϵ���ķ�ʽ;��Ż�������
			if (IMAGE_SNAP_BY_ORDINAL(pInt->u1.Ordinal))
			{
			}
			else { /*�����Ʒ�ʽ����*/
				// pInt->u1.AddressOfData�ֶε�ֵ��һ��IMAGE_IMPORT_BY_NAME
				// �ṹ���RVA
				IMAGE_IMPORT_BY_NAME* pImpName = 0;
				DWORD dwNameFoa = CTool::RVAtoFOA(pInt->u1.AddressOfData, CTool::pFileBuf);
				pImpName = (IMAGE_IMPORT_BY_NAME*)
					(dwNameFoa + CTool::pFileBuf);

				//ת����WideChar
				int num = MultiByteToWideChar(0, 0, pImpName->Name, -1, NULL, 0);
				wchar_t *wide = new wchar_t[num];
				MultiByteToWideChar(0, 0, pImpName->Name, -1, wide, num);
				//���뵽����
				m_delay_2.InsertItem(i, 0);
				//�������
				temp.Format(L"%x", pImpName->Hint);
				m_delay_2.SetItemText(i, 0, temp);
				//��������
				m_delay_2.SetItemText(i, 1, wide);
				//����ƫ��
				temp.Format(L"%x", pInt->u1.Function);
				m_delay_2.SetItemText(i, 2, temp);
			}
			++i;
			++pInt;
		}
		++pImp; // �ҵ���һ�������Ԫ��.
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
