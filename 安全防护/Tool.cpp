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

char* CTool::pFileBuf;
