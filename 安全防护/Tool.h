#pragma once
class CTool
{
public:
	CTool();
	~CTool();

	static char* GetFileMemPointer(CString& pFilePath);
	static bool IsPeFile(char* pFileBuf);
	static DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf);

	static char* pFileBuf;
};

