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

char* CTool::pFileBuf;
