// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
//1. 添加HOOK库的头文件.
#include "detours.h"
//64位就包含这个lib
//#pragma comment(lib,"detours/lib.X64/detours.lib")

//32位就包含这个lib
#pragma comment(lib,"detours/lib.X86/detours.lib")
DWORD g_dwProcessId = 0;

typedef BOOL(WINAPI* MyFnTerminateProcess)(HANDLE hProcess, UINT uExitCode);
MyFnTerminateProcess g_pSrcTerminateProcess = NULL;


DWORD OpenPID(){
	MessageBox(NULL, TEXT("进入OpenPID"), NULL, 0);
	
	// 1. 打开文件Mapping
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, L"PID");
	if (NULL == hMapFile){
		TCHAR msg[200] = {};
		wsprintf(msg, TEXT("ERROR: %d"), GetLastError());
		MessageBox(NULL, msg, NULL, 0);
		return 0;
	}
		
	
	// 2. 创建View
	MessageBox(NULL, TEXT("MapViewOfFile"), NULL, 0);
	PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 16);
	if (NULL == pBuf)
		return 0;

	g_dwProcessId = *((DWORD*)pBuf);
	//TCHAR msg[200] = {};
	//wsprintf(msg, TEXT("PID: %d"), g_dwProcessId);
	//MessageBox(NULL, msg, NULL, 0);


	// 5. 取消Mapping，关闭句柄
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}


// 3. 定义想要HOOK的API的函数原型.
// 定义函数类型
BOOL
WINAPI
MyTerminate(
_In_ HANDLE hProcess,
_In_ UINT uExitCode
)
{
	MessageBox(NULL, TEXT("进入MyTerminateProcess"), NULL, 0);
	OpenPID();
	if (GetProcessId(hProcess) == g_dwProcessId)
	{
		MessageBox(NULL, TEXT("关闭进程失败!"), TEXT("失败"), MB_ICONERROR);
		return FALSE;
	}
	return g_pSrcTerminateProcess(hProcess, uExitCode);
}




/*!
* \brief : hookFunction
* \return: LPVOID
* \param : LPVOID pSrcFcuntion 要被HOOK的API函数地址
* \param : LPVOID pDesFcuntion 要安装的钩子函数.
*/
LPVOID hookFunction(LPVOID pSrcFcuntion, LPVOID pDesFcuntion) {

	// 
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// 拦截函数,并把被钩住的的函数输出.
	// 作用: 
	// 将pSrcFcuntion所保存的函数地址进行HOOK
	// 并将被HOOK前的API输出到pSrcFcuntion
	DetourAttach(&pSrcFcuntion, pDesFcuntion);

	// 提交所有的更改(所有的HOOK)
	if (DetourTransactionCommit() == NO_ERROR)
		return pSrcFcuntion;// 返回原始版本的API地址
	return NULL;
}

void HookApi()
{
	MessageBox(NULL, TEXT("HOOK函数"), NULL, 0);
	// HOOK函数
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
		MessageBox(NULL, TEXT("注入成功!!!"), NULL, 0);
		HookApi();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:


		break;
	}
	return TRUE;
}

