// ResourceTable.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "ResourceTable.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "ResourceDescriptor.h"
//#include "BitmapPreviewViewDialog.h"
// CResourceTable 对话框

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


// CResourceTable 消息处理程序


BOOL CResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	ResourceTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	DecodeResourceEntry();

	CResourceDescriptor *pRootDescriptor = m_pResourceTable;
	AddNode(pRootDescriptor, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CResourceTable::AddNode(CResourceDescriptor * pDescriptor, HTREEITEM hParent)
{
	size_t ResourceCount = pDescriptor->NumberOfResources();
	for (size_t index = 0; index < ResourceCount; ++index)
	{
		CResourceDescriptor *pEntry = pDescriptor->GetResource(index);
		CString str;

		// 显示节点的名字
		GetNodeName(pEntry, str);

		// 插入新的节点
		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = hParent;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = str + "";

		// 得到当前项的句柄
		HTREEITEM hItem = ResourceTree.InsertItem(&tvInsert);

		// 用句柄和当前CResourceDescriptor对象关联
		m_TreeDataMap[hItem] = pEntry;

		// 如果是目录继续遍历, 目录小于2才往下遍历, 到时候需要处理下(让最后一个节点和父节点作为同一个节点来处理)
		if (pEntry->IsDir() && pEntry->GetDepth() < 2)
		{
			AddNode(pEntry, hItem);
		}
	}
}

void CResourceTable::DecodeResourceEntry()
{
	//获取NT头的指针
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//资源表的RVA
	DWORD dwResRVA = pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//RVA到FOA
	DWORD dwResFOA = CTool::RVAtoFOA(dwResRVA, CTool::pFileBuf);
	//资源表在文件中的起始位置
	PIMAGE_RESOURCE_DIRECTORY pRootDir = (PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + CTool::pFileBuf);

	// 构造一个树根 , CResourceDescriptor描述了每层资源
	//包括 : PIMAGE_RESOURCE_DIRECTORY pRawDirectory目录
	//PIMAGE_RESOURCE_DIRECTORY_ENTRY pRawDirectoryEntry实体
	//PIMAGE_RESOURCE_DATA_ENTRY pRawDataEntry真正的资源
	//CResourceDescriptor *pParent,
	//	PIMAGE_RESOURCE_DIRECTORY_ENTRY pRawDirectoryEntry,
	//	PIMAGE_RESOURCE_DIRECTORY pRawDirectory,
	//	PIMAGE_RESOURCE_DATA_ENTRY pRawDataEntry
	m_pResourceTable = new CResourceDescriptor(
		TEXT(""),
		NULL, //父亲
		NULL, //实际内容
		pRootDir,//儿子
		NULL //实际内容
		);

	//从树根开始遍历 , pRootDir是资源表的起始地址
	_DecodeResourceDirEntry(m_pResourceTable, pRootDir);

	CString WinName("资源表");
	SetWindowText(WinName);
}

void CResourceTable::_DecodeResourceDirEntry(CResourceDescriptor* pDirNode, PIMAGE_RESOURCE_DIRECTORY pDirInfo)
{
	_tstring TempName;
	//计算总的实体个数
	DWORD dwTotalCount = pDirInfo->NumberOfIdEntries + pDirInfo->NumberOfNamedEntries;
	//定位到第一个实体
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntries = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pDirInfo + 1);
	//遍历实体
	for (DWORD dwEntryIndex = 0; dwEntryIndex < dwTotalCount; ++dwEntryIndex)
	{
		TempName.clear();
		//通过下标获取实体对象
		IMAGE_RESOURCE_DIRECTORY_ENTRY& CurrentEntry = pEntries[dwEntryIndex];

		//判断是否是名称命名
		if (CurrentEntry.NameIsString)
		{
			// <名称>
			PIMAGE_RESOURCE_DIR_STRING_U pNameSU = (PIMAGE_RESOURCE_DIR_STRING_U)(pDirNode->GetRootAddress() + CurrentEntry.NameOffset);
			TempName.append(pNameSU->NameString, pNameSU->Length);
		}

		if (CurrentEntry.DataIsDirectory)
		{
			// 既然是目录, 就应该获取目录实体的指针数据
			PIMAGE_RESOURCE_DIRECTORY pCurrentDir = (PIMAGE_RESOURCE_DIRECTORY)(pDirNode->GetRootAddress() + CurrentEntry.OffsetToDirectory);

			// 构造新的目录节点
			CResourceDescriptor *pNewDirNode = new CResourceDescriptor(
				TempName,
				pDirNode,
				&CurrentEntry,
				pCurrentDir,
				NULL
				);

			// 将pNewDirNode加入到pDirNode中去
			pDirNode->AddResource(pNewDirNode);

			// 继续向深层次遍历
			_DecodeResourceDirEntry(pNewDirNode, pCurrentDir);
		}
		else
		{
			// 既然是文件, 就应该获取文件实体的指针数据
			PIMAGE_RESOURCE_DATA_ENTRY pCurrentData = (PIMAGE_RESOURCE_DATA_ENTRY)(pDirNode->GetRootAddress() + CurrentEntry.OffsetToData);

			// 创建节点
			CResourceDescriptor *pNewDataNode = new CResourceDescriptor(
				TempName,
				pDirNode,
				&CurrentEntry,
				NULL,
				pCurrentData
				);

			// 将数据节点加入到pDirNode中
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

		// 中间空出来
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
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;


	HTREEITEM hItem = ResourceTree.GetSelectedItem();

	// 取得指定节点的指针数据
	CResourceDescriptor *pDescriptor = m_TreeDataMap[hItem];

	// ** 类型为目录时, 需要设置一下(其实全时目录, 不判断也没关系)
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


	// 如果层次为2
	if (pDescriptor->GetDepth() == 2)
	{
		node_type = TEXT("<资源文件>");


		// 取得文件即可
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
		node_type = TEXT("<目录>");
		rva = "";
		foa = "";
	}

	UpdateData(FALSE);

}
