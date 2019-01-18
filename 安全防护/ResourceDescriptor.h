#pragma once
#include "stdafx.h"
#include <vector>

// 资源模块
class CResourceDescriptor
{
public:
	CResourceDescriptor(
		const _tstring& name,
		CResourceDescriptor *pParent,
		PIMAGE_RESOURCE_DIRECTORY_ENTRY pRawDirectoryEntry,
		PIMAGE_RESOURCE_DIRECTORY pRawDirectory,
		PIMAGE_RESOURCE_DATA_ENTRY pRawDataEntry
		) : m_pParent(NULL),
		m_pRawDirectoryEntry(NULL),
		m_pRawDirectory(NULL),
		m_pRawDataEntry(NULL),
		m_wDepth(0)
	{
		// 名字
		m_Name = name;

		// 父节点
		m_pParent = pParent;

		// 节点所在深度
		if (pParent != NULL)
			//父节点不为空 , 说明此节点是子节点
			m_wDepth = pParent->GetDepth() + 1;
		else
			m_dwRootAddress = (ULONGLONG)pRawDirectory;


		// DIR_ENTRY *
		if (pRawDirectoryEntry)
		{
			m_pRawDirectoryEntry = new IMAGE_RESOURCE_DIRECTORY_ENTRY{ 0 };
			*m_pRawDirectoryEntry = *pRawDirectoryEntry;
		}

		if (pRawDirectory)
		{
			// 说明当前节点是目录
			// DIR_ENTRY
			// DIR *
			m_pRawDirectory = new IMAGE_RESOURCE_DIRECTORY{ 0 };
			*m_pRawDirectory = *pRawDirectory;
		}
		else
		{
			// 说明当前节点不是目录
			// DIR_ENTRY
			// DATA_ENTRY *
			m_pRawDataEntry = new IMAGE_RESOURCE_DATA_ENTRY{ 0 };
			*m_pRawDataEntry = *pRawDataEntry;
		}

	}

	~CResourceDescriptor()
	{
		//SAFE_RELEASE(m_pRawDataEntry)
		//	SAFE_RELEASE(m_pRawDirectoryEntry)
		//	SAFE_RELEASE(m_pRawDirectory)
		//	CLEAR_DYN_VECTOR(m_Entries)
	}

public:
	bool IsDir() const { return m_pRawDirectory != NULL; }
	WORD GetDepth() const { return m_wDepth; }


	bool HasId() const { return GetId() > 0; }
	WORD GetId() const
	{
		if (m_pRawDirectoryEntry)
		{
			if (!m_pRawDirectoryEntry->NameIsString)
			{
				return m_pRawDirectoryEntry->Id;
			}
		}
		return 0;
	}

	bool HasName() const { return m_Name.size() > 0; }
	void GetName(_tstring& name) const
	{
		name += m_Name;
	}

	WORD GetAssetType() const
	{
		//RT_MANIFEST(24) => 0x18 (WinUser.h) 是最后一个可识别资源
		if (1 == m_wDepth && GetId() > 0 && GetId() <= 0x18)
		{
			return GetId();
		}
		return 0;
	}

	CResourceDescriptor *GetParent() const
	{
		return m_pParent;
	}

	ULONGLONG GetRootAddress() const
	{
		if (m_pParent)
			return m_pParent->GetRootAddress();
		return m_dwRootAddress;
	}

	// 添加新的资源
	void AddResource(CResourceDescriptor *pResource)
	{
		m_Entries.push_back(pResource);
	}

	CResourceDescriptor *GetResource(size_t index) const
	{
		return m_Entries[index];
	}

	size_t NumberOfResources() const
	{
		return m_Entries.size();
	}


	void GetRawDataEntry(PIMAGE_RESOURCE_DATA_ENTRY pDataEntry)
	{
		*pDataEntry = *m_pRawDataEntry;
	}

	void GetRawDirectoryEntry(PIMAGE_RESOURCE_DIRECTORY_ENTRY pDirEntry)
	{
		*pDirEntry = *m_pRawDirectoryEntry;
	}

	void GetRawDirectory(PIMAGE_RESOURCE_DIRECTORY pDirectory)
	{
		*pDirectory = *m_pRawDirectory;
	}

private:
	_tstring m_Name;						// 资源名字
	WORD m_wDepth;
	ULONGLONG m_dwRootAddress;					// 存放着根节点首地址(只有父节点才有)
	CResourceDescriptor* m_pParent;			// 父节点
	vector<CResourceDescriptor*> m_Entries;	// 子节点

	// >> 保存原始信息
	PIMAGE_RESOURCE_DATA_ENTRY m_pRawDataEntry;		 // 根据m_pRawDirectory计算获取的内容, 如果它不为NULL, 说明当前节点是文件, 非目录
	PIMAGE_RESOURCE_DIRECTORY_ENTRY m_pRawDirectoryEntry; // 这个不为NULL, 说明上一层是Entry, 但还需要判断才能知道当前这一层是文件还是目录, 如果为NULL, 说明是根节点
	PIMAGE_RESOURCE_DIRECTORY m_pRawDirectory;		 // 这个值不为NULL, 说明当前是一个目录

};

