// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
//1. ���HOOK���ͷ�ļ�.
#include "detours.h"
//64λ�Ͱ������lib
//#pragma comment(lib,"detours/lib.X64/detours.lib")

//32λ�Ͱ������lib
#pragma comment(lib,"detours/lib.X86/detours.lib")
DWORD g_dwProcessId = 0;

typedef BOOL(WINAPI* MyFnTerminateProcess)(HANDLE hProcess, UINT uExitCode);
MyFnTerminateProcess g_pSrcTerminateProcess = NULL;


DWORD OpenPID(){
	MessageBox(NULL, TEXT("����OpenPID"), NULL, 0);
	
	// 1. ���ļ�Mapping
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, L"PID");
	if (NULL == hMapFile){
		TCHAR msg[200] = {};
		wsprintf(msg, TEXT("ERROR: %d"), GetLastError());
		MessageBox(NULL, msg, NULL, 0);
		return 0;
	}
		
	
	// 2. ����View
	MessageBox(NULL, TEXT("MapViewOfFile"), NULL, 0);
	PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 16);
	if (NULL == pBuf)
		return 0;

	g_dwProcessId = *((DWORD*)pBuf);
	//TCHAR msg[200] = {};
	//wsprintf(msg, TEXT("PID: %d"), g_dwProcessId);
	//MessageBox(NULL, msg, NULL, 0);


	// 5. ȡ��Mapping���رվ��
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}


// 3. ������ҪHOOK��API�ĺ���ԭ��.
// ���庯������
BOOL
WINAPI
MyTerminate(
_In_ HANDLE hProcess,
_In_ UINT uExitCode
)
{
	MessageBox(NULL, TEXT("����MyTerminateProcess"), NULL, 0);
	OpenPID();
	if (GetProcessId(hProcess) == g_dwProcessId)
	{
		MessageBox(NULL, TEXT("�رս���ʧ��!"), TEXT("ʧ��"), MB_ICONERROR);
		return FALSE;
	}
	return g_pSrcTerminateProcess(hProcess, uExitCode);
}




/*!
* \brief : hookFunction
* \return: LPVOID
* \param : LPVOID pSrcFcuntion Ҫ��HOOK��API������ַ
* \param : LPVOID pDesFcuntion Ҫ��װ�Ĺ��Ӻ���.
*/
LPVOID hookFunction(LPVOID pSrcFcuntion, LPVOID pDesFcuntion) {

	// 
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// ���غ���,���ѱ���ס�ĵĺ������.
	// ����: 
	// ��pSrcFcuntion������ĺ�����ַ����HOOK
	// ������HOOKǰ��API�����pSrcFcuntion
	DetourAttach(&pSrcFcuntion, pDesFcuntion);

	// �ύ���еĸ���(���е�HOOK)
	if (DetourTransactionCommit() == NO_ERROR)
		return pSrcFcuntion;// ����ԭʼ�汾��API��ַ
	return NULL;
}

void HookApi()
{
	MessageBox(NULL, TEXT("HOOK����"), NULL, 0);
	// HOOK����
	g_pSrcTerminateProcess =
		(MyFnTerminateProcess)hookFunction(&TerminateProcess, &MyTerminate);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, TEXT("ע��ɹ�!!!"), NULL, 0);
		HookApi();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:


		break;
	}
	return TRUE;
}

