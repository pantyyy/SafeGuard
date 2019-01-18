#pragma once
#include "stdafx.h"
#include <vector>

// ��Դģ��
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
		// ����
		m_Name = name;

		// ���ڵ�
		m_pParent = pParent;

		// �ڵ��������
		if (pParent != NULL)
			//���ڵ㲻Ϊ�� , ˵���˽ڵ����ӽڵ�
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
			// ˵����ǰ�ڵ���Ŀ¼
			// DIR_ENTRY
			// DIR *
			m_pRawDirectory = new IMAGE_RESOURCE_DIRECTORY{ 0 };
			*m_pRawDirectory = *pRawDirectory;
		}
		else
		{
			// ˵����ǰ�ڵ㲻��Ŀ¼
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
		//RT_MANIFEST(24) => 0x18 (WinUser.h) �����һ����ʶ����Դ
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

	// ����µ���Դ
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
	_tstring m_Name;						// ��Դ����
	WORD m_wDepth;
	ULONGLONG m_dwRootAddress;					// ����Ÿ��ڵ��׵�ַ(ֻ�и��ڵ����)
	CResourceDescriptor* m_pParent;			// ���ڵ�
	vector<CResourceDescriptor*> m_Entries;	// �ӽڵ�

	// >> ����ԭʼ��Ϣ
	PIMAGE_RESOURCE_DATA_ENTRY m_pRawDataEntry;		 // ����m_pRawDirectory�����ȡ������, �������ΪNULL, ˵����ǰ�ڵ����ļ�, ��Ŀ¼
	PIMAGE_RESOURCE_DIRECTORY_ENTRY m_pRawDirectoryEntry; // �����ΪNULL, ˵����һ����Entry, ������Ҫ�жϲ���֪����ǰ��һ�����ļ�����Ŀ¼, ���ΪNULL, ˵���Ǹ��ڵ�
	PIMAGE_RESOURCE_DIRECTORY m_pRawDirectory;		 // ���ֵ��ΪNULL, ˵����ǰ��һ��Ŀ¼

};

