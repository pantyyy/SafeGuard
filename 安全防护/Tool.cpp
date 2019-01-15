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
	//pFilePath为文件路径
	//根据文集路径 , 获取文件句柄
	HANDLE hFile = CreateFile(
		FilePath,
		GENERIC_WRITE | GENERIC_READ,/*文件的读写模式*/
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	//判断文件是否打开成功
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("文件打开失败\n");
		return NULL;
	}

	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	//申请内存 , pBuf指向了申请内存的首地址
	char* pBuf = new char[dwFileSize]{};

	//读取文件
	DWORD dwRead;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

	//关闭文件
	CloseHandle(hFile);

	//返回基址
	return pBuf;
}

bool CTool::IsPeFile(char* pFileBuf)
{
	//获取DOS头指针
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	//判断MZ标识
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}

	//获取NT头指针 , NT头起始位置 = 基址 + Dos头的大小
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pFileBuf + pDos->e_lfanew);

	//判断PE指纹标识
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}

	return true;
}

DWORD CTool::RVAtoFOA(DWORD dwRVA, char* pFileBuf)
{
	//RVA - 区段在内存中的起始位置 = FOA - 区段在文件中的起始位置
	//RVA - VOffset = FOA - ROffset
	//RVA = FOA - ROffset + VOffset

	//遍历区段表获取相关数据
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + pFileBuf);

	//获取区段个数
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//获取区段表的起始地址
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	//遍历每个区段表
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//判断落在了哪个区段
		//区段的起始位置 + 区段在文件中对齐之后的大小
		if (dwRVA >= pSec->VirtualAddress &&
			dwRVA < pSec->VirtualAddress + pSec->SizeOfRawData)
		{
			return dwRVA -
				pSec->VirtualAddress +
				pSec->PointerToRawData;
		}

		//移动到下一个区段表
		pSec++;
	}


	return 0;
}

//获取所有的文件
void CTool::GetAllFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList)
{
	WCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA w32FindData;
	FileInfo fileInfo;
	StringCchPrintf(szFind, MAX_PATH, L"%s\\*", TargetPath);

	//查找第一个文件
	HANDLE hFindFile = ::FindFirstFile(szFind, &w32FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
		return;

	//遍历文件夹
	do
	{
		//去掉两个特殊的目录
		if ((!wcscmp(w32FindData.cFileName, L".")) || (!wcscmp(w32FindData.cFileName, L"..")))
		{
			continue;
		}

		//判断是否是文件夹
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//是文件夹
			//拼接路径
			StringCchPrintf(szFind, MAX_PATH, L"%s\\", TargetPath);//追加反斜杠
			StringCchCat(szFind, MAX_PATH, w32FindData.cFileName);//子目录名
			GetAllFile(szFind, fileList);
		}
		else{
			//是文件
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


//获取所有垃圾文件
void CTool::GetAllTrashFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList)
{
	WCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA w32FindData;
	FileInfo fileInfo;
	StringCchPrintf(szFind, MAX_PATH, L"%s\\*", TargetPath);

	//查找第一个文件
	HANDLE hFindFile = ::FindFirstFile(szFind, &w32FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
		return;

	//遍历文件夹
	do
	{
		//去掉两个特殊的目录
		if ((!wcscmp(w32FindData.cFileName, L".")) || (!wcscmp(w32FindData.cFileName, L"..")))
		{
			continue;
		}

		//判断是否是文件夹
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//是文件夹
			//拼接路径
			StringCchPrintf(szFind, MAX_PATH, L"%s\\", TargetPath);//追加反斜杠
			StringCchCat(szFind, MAX_PATH, w32FindData.cFileName);//子目录名
			GetAllTrashFile(szFind, fileList);
		}
		else{
			//是文件

			//获取文件的完整路径
			WCHAR szFullName[MAX_PATH];
			StringCchPrintf(szFullName, MAX_PATH, L"%s\\", TargetPath);
			StringCchCat(szFullName, MAX_PATH, w32FindData.cFileName);
			//获取文件的后缀
			LPWSTR FileSuffix = PathFindExtension(szFullName);

			//比较后缀名
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
