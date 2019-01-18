// Delay.cpp : 实现文件
//

#include "stdafx.h"
#include "安全防护.h"
#include "Delay.h"
#include "afxdialogex.h"
#include "Tool.h"

// CDelay 对话框

IMPLEMENT_DYNAMIC(CDelay, CDialogEx)

CDelay::CDelay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDelay::IDD, pParent)
{

}

CDelay::~CDelay()
{
}

void CDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_delay_1);
	DDX_Control(pDX, IDC_LIST2, m_delay_2);
}


BEGIN_MESSAGE_MAP(CDelay, CDialogEx)
END_MESSAGE_MAP()


// CDelay 消息处理程序


BOOL CDelay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString WinTitle("延迟加载表");
	SetWindowText(WinTitle);

	m_delay_1.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_delay_1.InsertColumn(0, L"Dll名称", 0, 100);
	m_delay_1.InsertColumn(1, L"IAT RVA", 0, 50);
	m_delay_1.InsertColumn(2, L"INA RVA", 0, 50);
	m_delay_1.InsertColumn(3, L"绑定IAT RVA", 0, 200);
	m_delay_1.InsertColumn(4, L"卸载函数 RVA", 0, 200);
	//初始化导入表列表
	m_delay_2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_delay_2.InsertColumn(0, L"序号", 0, 50);
	m_delay_2.InsertColumn(1, L"名称", 0, 300);
	m_delay_2.InsertColumn(2, L"偏移地址", 0, 100);

	//获取PE头对象
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)CTool::pFileBuf;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + CTool::pFileBuf);

	//第一步: 先通过扩展头找到数据目录的第1项, 获取导入表的首地址.
	DWORD dwImpTabRva = pNt->OptionalHeader.DataDirectory[13].VirtualAddress;

	//1.1 因为文件没有内存中拉伸展开, 所以不能直接使用rva
	//    定位文件中的内容, 因此,需要将rva转换成文件偏移去定位.
	DWORD dwImpTabFoa = CTool::RVAtoFOA(dwImpTabRva, CTool::pFileBuf);
	// 得到的是一个导入表数组的首地址
	_IMAGE_DELAYLOAD_DESCRIPTOR* pImp =
		(_IMAGE_DELAYLOAD_DESCRIPTOR*)(dwImpTabFoa + CTool::pFileBuf);
	// 	if (pImp==NULL)
	// 	{
	// 		MessageBox(L"没有延迟加载");
	// 	}
	nItem = -1;
	int  i = 0;
	CString temp;
	//第二步 : 遍历整个导入表数组, 数组是以一个全零元素作为结尾.
	while (pImp->DllNameRVA != 0)
	{
		//第三步 : 在遍历出每一个导入表结构体时, 遍历它的INT或IAT.
		// 3.1 获取到导入表的dll的名字
		DWORD dwNameFoa = CTool::RVAtoFOA(pImp->DllNameRVA, CTool::pFileBuf); // Name也是rva,需要转换为foa
		char* pDllName = (char*)(dwNameFoa + CTool::pFileBuf);
		//转换成WideChar
		int num = MultiByteToWideChar(0, 0, pDllName, -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(0, 0, pDllName, -1, wide, num);

		//插入dll名称
		m_delay_1.InsertItem(i, NULL);
		m_delay_1.SetItemText(i, 0, wide);

		// 3.2 从INT/IAT中得到所有的导入函数的名字/序号
		IMAGE_THUNK_DATA* pIat = 0, *pInt = 0;
		DWORD dwIntFoa = CTool::RVAtoFOA(pImp->ImportNameTableRVA, CTool::pFileBuf); // INT数组的rva
		pInt = (IMAGE_THUNK_DATA*)(dwIntFoa + CTool::pFileBuf);
		temp.Format(L"%x", pImp->ImportNameTableRVA);
		m_delay_1.SetItemText(i, 1, temp);

		DWORD dwIatFoa = CTool::RVAtoFOA(pImp->ImportAddressTableRVA, CTool::pFileBuf); // INT数组的rva
		pIat = (IMAGE_THUNK_DATA*)(dwIatFoa + CTool::pFileBuf);
		temp.Format(L"%x", pImp->ImportAddressTableRVA);
		m_delay_1.SetItemText(i, 2, temp);

		temp.Format(L"%x", pImp->BoundImportAddressTableRVA);
		m_delay_1.SetItemText(i, 3, temp);

		temp.Format(L"%x", pImp->UnloadInformationTableRVA);
		m_delay_1.SetItemText(i, 4, temp);

		++i;
		++pImp; // 找到下一个导入表元素.
		nItem++;
	}
	//——————————————————————————————————————————————————
	if (nItem != -1)
	{
		//第一步: 先通过扩展头找到数据目录的第1项, 获取导入表的首地址.
		DWORD dwImpTabRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;

		//1.1 因为文件没有内存中拉伸展开, 所以不能直接使用rva
		//    定位文件中的内容, 因此,需要将rva转换成文件偏移去定位.
		DWORD dwImpTabFoa = CTool::RVAtoFOA(dwImpTabRva, CTool::pFileBuf);
		// 得到的是一个导入表数组的首地址
		_IMAGE_DELAYLOAD_DESCRIPTOR* pImp =
			(_IMAGE_DELAYLOAD_DESCRIPTOR*)(dwImpTabFoa + CTool::pFileBuf);
		int  i = 0;
		CString temp;
		//获取地址
		pImp += nItem;

		// 3.2 从INT/IAT中得到所有的导入函数的名字/序号
		IMAGE_THUNK_DATA32* pIat = 0, *pInt = 0;
		DWORD dwIntFoa = CTool::RVAtoFOA(pImp->ImportNameTableRVA, CTool::pFileBuf); // INT数组的rva
		pInt = (IMAGE_THUNK_DATA32*)(dwIntFoa + CTool::pFileBuf);

		DWORD dwIatFoa = CTool::RVAtoFOA(pImp->ImportAddressTableRVA, CTool::pFileBuf); // INT数组的rva
		pIat = (IMAGE_THUNK_DATA32*)(dwIatFoa + CTool::pFileBuf);
		//使用其中一个表就可以得到导入函数的信息了
		//得到的INT是一个IMAGE_THUNK_DATA的数组,以
		//全0元素作为结尾.
		while (pInt->u1.Function != 0)
		{
			// 判断导入的方式;序号还是名称
			if (IMAGE_SNAP_BY_ORDINAL(pInt->u1.Ordinal))
			{
			}
			else { /*以名称方式导入*/
				// pInt->u1.AddressOfData字段的值是一个IMAGE_IMPORT_BY_NAME
				// 结构体的RVA
				IMAGE_IMPORT_BY_NAME* pImpName = 0;
				DWORD dwNameFoa = CTool::RVAtoFOA(pInt->u1.AddressOfData, CTool::pFileBuf);
				pImpName = (IMAGE_IMPORT_BY_NAME*)
					(dwNameFoa + CTool::pFileBuf);

				//转换成WideChar
				int num = MultiByteToWideChar(0, 0, pImpName->Name, -1, NULL, 0);
				wchar_t *wide = new wchar_t[num];
				MultiByteToWideChar(0, 0, pImpName->Name, -1, wide, num);
				//插入到里面
				m_delay_2.InsertItem(i, 0);
				//插入序号
				temp.Format(L"%x", pImpName->Hint);
				m_delay_2.SetItemText(i, 0, temp);
				//插入名称
				m_delay_2.SetItemText(i, 1, wide);
				//插入偏移
				temp.Format(L"%x", pInt->u1.Function);
				m_delay_2.SetItemText(i, 2, temp);
			}
			++i;
			++pInt;
		}
		++pImp; // 找到下一个导入表元素.
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
