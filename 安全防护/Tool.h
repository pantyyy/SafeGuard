#pragma once
#include<vector>
#include <Strsafe.h>
#include "ClientSocket.h"
#include<string>
#include<vector>
using namespace std;


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
	static void SplitString(const string& s, vector<string>& v, const string& c);


	static char* pFileBuf;

	static CClientSocket m_client; 
};

