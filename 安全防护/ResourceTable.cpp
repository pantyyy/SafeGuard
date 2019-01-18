// ResourceTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "ResourceTable.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "ResourceDescriptor.h"
//#include "BitmapPreviewViewDialog.h"
// CResourceTable �Ի���

IMPLEMENT_DYNAMIC(CResourceTable, CDialogEx)

CResourceTable::CResourceTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResourceTable::IDD, pParent)
	, num_of_str(_T(""))
	, num_of_ID(_T(""))
	, node_type(_T(""))
	, rva(_T(""))
	, foa(_T(""))
{

}

CResourceTable::~CResourceTable()
{
}

void CResourceTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, ResourceTree);
	DDX_Text(pDX, IDC_EDIT1, num_of_str);
	DDX_Text(pDX, IDC_EDIT4, num_of_ID);
	DDX_Text(pDX, IDC_EDIT5, node_type);
	DDX_Text(pDX, IDC_EDIT18, rva);
	DDX_Text(pDX, IDC_EDIT19, foa);
}


BEGIN_MESSAGE_MAP(CResourceTable, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CResourceTable::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CResourceTable ��Ϣ�������


BOOL CResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	ResourceTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	DecodeResourceEntry();

	CResourceDescriptor *pRootDescriptor = m_pResourceTable;
	AddNode(pRootDescriptor, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CResourceTable::AddNode(CResourceDescriptor * pDescriptor, HTREEITEM hParent)
{
	size_t ResourceCount = pDescriptor->NumberOfResources();
	for (size_t index = 0; index < ResourceCount; ++index)
	{
		CResourceDescriptor *pEntry = pDescriptor->GetResource(index);
		CString str;

		// ��ʾ�ڵ������
		GetNodeName(pEntry, str);

		// �����µĽڵ�
		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = hParent;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = str + "";

		// �õ���ǰ��ľ��
		HTREEITEM hItem = ResourceTree.InsertItem(&tvInsert);

		// �þ���͵�ǰCResourceDescriptor�������
		m_TreeDataMap[hItem] = pEntry;

		// �����Ŀ¼��������, Ŀ¼С��2�����±���, ��ʱ����Ҫ������(�����һ���ڵ�͸��ڵ���Ϊͬһ���ڵ�������)
		if (pEntry->IsDir() && pEntry->GetDepth() < 2)
		{
			AddNode(pEntry, hItem);
		}
	}
}

void CResourceTable::DecodeResourceEntry()
{
	//��ȡNTͷ��ָ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//��Դ���RVA
	DWORD dwResRVA = pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//RVA��FOA
	DWORD dwResFOA = CTool::RVAtoFOA(dwResRVA, CTool::pFileBuf);
	//��Դ�����ļ��е���ʼλ��
	PIMAGE_RESOURCE_DIRECTORY pRootDir = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CTool::pFileBuf);

	// ����һ������ , CResourceDescriptor������ÿ����Դ
	//���� : PIMAGE_RESOURCE_DIRECTORY pRawDirectoryĿ¼
	//PIMAGE_RESOURCE_DIRECTORY_ENTRY pRawDirectoryEntryʵ��
	//PIMAGE_RESOURCE_DATA_ENTRY pRawDataEntry��������Դ
	//CResourceDescriptor *pParent,
	//	PIMAGE_RESOURCE_DIRECTORY_ENTRY pRawDirectoryEntry,
	//	PIMAGE_RESOURCE_DIRECTORY pRawDirectory,
	//	PIMAGE_RESOURCE_DATA_ENTRY pRawDataEntry
	m_pResourceTable = new CResourceDescriptor(
		TEXT(""),
		NULL, //����
		NULL, //ʵ������
		pRootDir,//����
		NULL //ʵ������
		);

	//��������ʼ���� , pRootDir����Դ�����ʼ��ַ
	_DecodeResourceDirEntry(m_pResourceTable, pRootDir);

	CString WinName("��Դ��");
	SetWindowText(WinName);
}

void CResourceTable::_DecodeResourceDirEntry(CResourceDescriptor* pDirNode, PIMAGE_RESOURCE_DIRECTORY pDirInfo)
{
	_tstring TempName;
	//�����ܵ�ʵ�����
	DWORD dwTotalCount = pDirInfo->NumberOfIdEntries + pDirInfo->NumberOfNamedEntries;
	//��λ����һ��ʵ��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntries = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pDirInfo + 1);
	//����ʵ��
	for (DWORD dwEntryIndex = 0; dwEntryIndex < dwTotalCount; ++dwEntryIndex)
	{
		TempName.clear();
		//ͨ���±��ȡʵ�����
		IMAGE_RESOURCE_DIRECTORY_ENTRY& CurrentEntry = pEntries[dwEntryIndex];

		//�ж��Ƿ�����������
		if (CurrentEntry.NameIsString)
		{
			// <����>
			PIMAGE_RESOURCE_DIR_STRING_U pNameSU = (PIMAGE_RESOURCE_DIR_STRING_U)(pDirNode->GetRootAddress() + CurrentEntry.NameOffset);
			TempName.append(pNameSU->NameString, pNameSU->Length);
		}

		if (CurrentEntry.DataIsDirectory)
		{
			// ��Ȼ��Ŀ¼, ��Ӧ�û�ȡĿ¼ʵ���ָ������
			PIMAGE_RESOURCE_DIRECTORY pCurrentDir = (PIMAGE_RESOURCE_DIRECTORY)(pDirNode->GetRootAddress() + CurrentEntry.OffsetToDirectory);

			// �����µ�Ŀ¼�ڵ�
			CResourceDescriptor *pNewDirNode = new CResourceDescriptor(
				TempName,
				pDirNode,
				&CurrentEntry,
				pCurrentDir,
				NULL
				);

			// ��pNewDirNode���뵽pDirNode��ȥ
			pDirNode->AddResource(pNewDirNode);

			// ���������α���
			_DecodeResourceDirEntry(pNewDirNode, pCurrentDir);
		}
		else
		{
			// ��Ȼ���ļ�, ��Ӧ�û�ȡ�ļ�ʵ���ָ������
			PIMAGE_RESOURCE_DATA_ENTRY pCurrentData = (PIMAGE_RESOURCE_DATA_ENTRY)(pDirNode->GetRootAddress() + CurrentEntry.OffsetToData);

			// �����ڵ�
			CResourceDescriptor *pNewDataNode = new CResourceDescriptor(
				TempName,
				pDirNode,
				&CurrentEntry,
				NULL,
				pCurrentData
				);

			// �����ݽڵ���뵽pDirNode��
			pDirNode->AddResource(pNewDataNode);
		}
	}
}

void CResourceTable::GetNodeName(CResourceDescriptor *pDescriptor, CString& str)
{
	if (pDescriptor->GetAssetType() > 0)
	{
		GetInternalDisplayName(pDescriptor->GetAssetType(), str);
	}
	else if (pDescriptor->HasName())
	{
		_tstring nodeName;
		pDescriptor->GetName(nodeName);
		str.Append(TEXT("\""));
		str += nodeName.c_str();
		str.Append(TEXT("\""));
	}
	else
	{
		str.Format(TEXT("%d"), pDescriptor->GetId());
	}
}

void CResourceTable::GetInternalDisplayName(WORD wType, CString & str)
{
	switch (wType)
	{
	case 1:
		str = TEXT("Cursor");
		break;

	case 2:
		str = TEXT("Bitmap");
		break;

	case 3:
		str = TEXT("Icon");
		break;

	case 4:
		str = TEXT("Menu");
		break;

	case 5:
		str = TEXT("Dialog");
		break;

	case 6:
		str = TEXT("String");
		break;

	case 7:
		str = TEXT("Font Dir");
		break;

	case 8:
		str = TEXT("Font");
		break;

	case 9:
		str = TEXT("Accelerator");
		break;

	case 10:
		str = TEXT("RCData");
		break;

	case 11:
		str = TEXT("Message Table");
		break;

	case 12:
		str = TEXT("Group Icon");
		break;

		// �м�ճ���
	case 14:
		str = TEXT("Group Icon");
		break;

	case 16:
		str = TEXT("Version");
		break;

	case 17:
		str = TEXT("Dlg Include");
		break;

	case 19:
		str = TEXT("Plu Play");
		break;

	case 20:
		str = TEXT("VXD");
		break;

	case 21:
		str = TEXT("ANI Cursor");
		break;

	case 22:
		str = TEXT("ANI Icon");
		break;

	case 23:
		str = TEXT("Html");
		break;

	case 24:
		str = TEXT("Manifest");
		break;

	default:
		str = TEXT("<Unknown>");
		break;
	}
}


void CResourceTable::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	HTREEITEM hItem = ResourceTree.GetSelectedItem();

	// ȡ��ָ���ڵ��ָ������
	CResourceDescriptor *pDescriptor = m_TreeDataMap[hItem];

	// ** ����ΪĿ¼ʱ, ��Ҫ����һ��(��ʵȫʱĿ¼, ���ж�Ҳû��ϵ)
	//if (pDescriptor->IsDir())
	//{

	IMAGE_RESOURCE_DIRECTORY currentDirectory;
	pDescriptor->GetRawDirectory(&currentDirectory);

	IMAGE_RESOURCE_DIRECTORY_ENTRY currentDirEntry;
	pDescriptor->GetRawDirectoryEntry(&currentDirEntry);

	CString str;

	str.Format(TEXT("%04X"), currentDirectory.NumberOfNamedEntries);
	num_of_str = str;

	str.Format(TEXT("%04X"), currentDirectory.NumberOfIdEntries);
	num_of_ID = str;


	// ������Ϊ2
	if (pDescriptor->GetDepth() == 2)
	{
		node_type = TEXT("<��Դ�ļ�>");


		// ȡ���ļ�����
		IMAGE_RESOURCE_DATA_ENTRY dataEntry;
		CResourceDescriptor *pData = pDescriptor->GetResource(0);
		pData->GetRawDataEntry(&dataEntry);

		// RVA
		str.Format(TEXT("%08X"), dataEntry.OffsetToData);
		rva = str;

		// FOA
		str.Format(TEXT("%08X"), CTool::RVAtoFOA(dataEntry.OffsetToData , CTool::pFileBuf));
		foa = str;

		WORD assetType = pDescriptor->GetParent()->GetAssetType();

	}
	else
	{
		node_type = TEXT("<Ŀ¼>");
		rva = "";
		foa = "";
	}

	UpdateData(FALSE);

}
