#pragma once
#include<vector>
#include <Strsafe.h>

struct FileInfo {
	FILETIME ftCreationTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	WCHAR  cFileName[MAX_PATH];
	WCHAR  cFilePath[MAX_PATH];

};


class CTool
{
public:
	CTool();
	~CTool();

	static char* GetFileMemPointer(CString& pFilePath);
	static bool IsPeFile(char* pFileBuf);
	static DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf);

	static void GetAllFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList);
	static void GetAllTrashFile(WCHAR* TargetPath, std::vector<FileInfo> &fileList);

	static char* pFileBuf;
};

