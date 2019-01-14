// SectionInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "SectionInfo.h"
#include "afxdialogex.h"
#include "Tool.h"

// CSectionInfo �Ի���

IMPLEMENT_DYNAMIC(CSectionInfo, CDialogEx)

CSectionInfo::CSectionInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSectionInfo::IDD, pParent)
{

}

CSectionInfo::~CSectionInfo()
{
}

void CSectionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, SectionInfoList);
}


BEGIN_MESSAGE_MAP(CSectionInfo, CDialogEx)
END_MESSAGE_MAP()


// CSectionInfo ��Ϣ�������


BOOL CSectionInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ô�����
	CString WinTitle("������Ϣ");
	SetWindowText(WinTitle.GetBuffer());

	//���ñ�ͷ
	ModifyStyleEx(0, WS_EX_APPWINDOW);

	SectionInfoList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	SectionInfoList.InsertColumn(0, L"����", 0, 70);
	SectionInfoList.InsertColumn(1, L"VOffset", 0, 70);
	SectionInfoList.InsertColumn(2, L"VSize", 0, 70);
	SectionInfoList.InsertColumn(3, L"ROffset", 0, 70);
	SectionInfoList.InsertColumn(4, L"RSize", 0, 70);
	SectionInfoList.InsertColumn(5, L"��־", 0, 70);

	//��λ����һ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNt);

	
	//����ÿһ�������
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		CString tempStr(pSectionHeader->Name);
		SectionInfoList.InsertItem(i, L"");

		//����
		SectionInfoList.SetItemText(i, 0, tempStr);

		//VOffset
		tempStr.Format(L"%04X", pSectionHeader->VirtualAddress);
		SectionInfoList.SetItemText(i, 1, tempStr);

		//VSize
		tempStr.Format(L"%04X", pSectionHeader->Misc);
		SectionInfoList.SetItemText(i, 2, tempStr);

		//ROffset
		tempStr.Format(L"%04X", pSectionHeader->PointerToRawData);
		SectionInfoList.SetItemText(i, 3, tempStr);

		//RSize
		tempStr.Format(L"%04X", pSectionHeader->SizeOfRawData);
		SectionInfoList.SetItemText(i, 4, tempStr);

		//��־
		tempStr.Format(L"%04X", pSectionHeader->Characteristics);
		SectionInfoList.SetItemText(i, 5, tempStr);

		pSectionHeader++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
