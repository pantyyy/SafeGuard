#include "stdafx.h"
#include "Tool.h"




CTool::CTool()
{
}


CTool::~CTool()
{
}


char* CTool::GetFileMemPointer(CString& FilePath)
{
	//pFilePathΪ�ļ�·��
	//�����ļ�·�� , ��ȡ�ļ����
	HANDLE hFile = CreateFile(
		FilePath,
		GENERIC_WRITE | GENERIC_READ,/*�ļ��Ķ�дģʽ*/
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	//�ж��ļ��Ƿ�򿪳ɹ�
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("�ļ���ʧ��\n");
		return NULL;
	}

	//��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	//�����ڴ� , pBufָ���������ڴ���׵�ַ
	char* pBuf = new char[dwFileSize]{};

	//��ȡ�ļ�
	DWORD dwRead;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

	//�ر��ļ�
	CloseHandle(hFile);

	//���ػ�ַ
	return pBuf;
}

bool CTool::IsPeFile(char* pFileBuf)
{
	//��ȡDOSͷָ��
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	//�ж�MZ��ʶ
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}

	//��ȡNTͷָ�� , NTͷ��ʼλ�� = ��ַ + Dosͷ�Ĵ�С
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pFileBuf + pDos->e_lfanew);

	//�ж�PEָ�Ʊ�ʶ
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}

	return true;
}

DWORD CTool::RVAtoFOA(DWORD dwRVA, char* pFileBuf)
{
	//RVA - �������ڴ��е���ʼλ�� = FOA - �������ļ��е���ʼλ��
	//RVA - VOffset = FOA - ROffset
	//RVA = FOA - ROffset + VOffset

	//�������α��ȡ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + pFileBuf);

	//��ȡ���θ���
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//��ȡ���α����ʼ��ַ
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	//����ÿ�����α�
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//�ж��������ĸ�����
		//���ε���ʼλ�� + �������ļ��ж���֮��Ĵ�С
		if (dwRVA >= pSec->VirtualAddress &&
			dwRVA < pSec->VirtualAddress + pSec->SizeOfRawData)
		{
			return dwRVA -
				pSec->VirtualAddress +
				pSec->PointerToRawData;
		}

		//�ƶ�����һ�����α�
		pSec++;
	}


	return 0;
}

//��ȡ���е��ļ�
void CTool::GetAllFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList)
{
	WCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA w32FindData;
	FileInfo fileInfo;
	StringCchPrintf(szFind, MAX_PATH, L"%s\\*", TargetPath);

	//���ҵ�һ���ļ�
	HANDLE hFindFile = ::FindFirstFile(szFind, &w32FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
		return;

	//�����ļ���
	do
	{
		//ȥ�����������Ŀ¼
		if ((!wcscmp(w32FindData.cFileName, L".")) || (!wcscmp(w32FindData.cFileName, L"..")))
		{
			continue;
		}

		//�ж��Ƿ����ļ���
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//���ļ���
			//ƴ��·��
			StringCchPrintf(szFind, MAX_PATH, L"%s\\", TargetPath);//׷�ӷ�б��
			StringCchCat(szFind, MAX_PATH, w32FindData.cFileName);//��Ŀ¼��
			GetAllFile(szFind, fileList);
		}
		else{
			//���ļ�
			WCHAR szFullName[MAX_PATH];
			StringCchPrintf(szFullName , MAX_PATH , L"%s\\" , TargetPath);
			StringCchCat(szFullName , MAX_PATH , w32FindData.cFileName);

			StringCchPrintf(fileInfo.cFileName, MAX_PATH, L"%s", w32FindData.cFileName);
			StringCchPrintf(fileInfo.cFilePath, MAX_PATH, L"%s", szFullName);
			fileInfo.nFileSizeHigh = w32FindData.nFileSizeHigh;
			fileInfo.nFileSizeLow = w32FindData.nFileSizeLow;

			fileList.push_back(fileInfo);
		}

	} while (FindNextFile(hFindFile, &w32FindData));
}


//��ȡ���������ļ�
void CTool::GetAllTrashFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList)
{
	WCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA w32FindData;
	FileInfo fileInfo;
	StringCchPrintf(szFind, MAX_PATH, L"%s\\*", TargetPath);

	//���ҵ�һ���ļ�
	HANDLE hFindFile = ::FindFirstFile(szFind, &w32FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
		return;

	//�����ļ���
	do
	{
		//ȥ�����������Ŀ¼
		if ((!wcscmp(w32FindData.cFileName, L".")) || (!wcscmp(w32FindData.cFileName, L"..")))
		{
			continue;
		}

		//�ж��Ƿ����ļ���
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//���ļ���
			//ƴ��·��
			StringCchPrintf(szFind, MAX_PATH, L"%s\\", TargetPath);//׷�ӷ�б��
			StringCchCat(szFind, MAX_PATH, w32FindData.cFileName);//��Ŀ¼��
			GetAllTrashFile(szFind, fileList);
		}
		else{
			//���ļ�

			//��ȡ�ļ�������·��
			WCHAR szFullName[MAX_PATH];
			StringCchPrintf(szFullName, MAX_PATH, L"%s\\", TargetPath);
			StringCchCat(szFullName, MAX_PATH, w32FindData.cFileName);
			//��ȡ�ļ��ĺ�׺
			LPWSTR FileSuffix = PathFindExtension(szFullName);

			//�ȽϺ�׺��
			if (!lstrcmp(FileSuffix, L".tlog") || !lstrcmp(FileSuffix, L".obj") || 
				!lstrcmp(FileSuffix, L".log")|| !lstrcmp(FileSuffix, L".ilk") || 
				!lstrcmp(FileSuffix, L".pdb") || !lstrcmp(FileSuffix, L".tmp"))
			{
				StringCchPrintf(fileInfo.cFileName, MAX_PATH, L"%s", w32FindData.cFileName);
				StringCchPrintf(fileInfo.cFilePath, MAX_PATH, L"%s", szFullName);
				fileInfo.nFileSizeHigh = w32FindData.nFileSizeHigh;
				fileInfo.nFileSizeLow = w32FindData.nFileSizeLow;

				fileList.push_back(fileInfo);
			}


		}

	} while (FindNextFile(hFindFile, &w32FindData));
}

char* CTool::pFileBuf;
