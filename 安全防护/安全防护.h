
// ��ȫ����.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C��ȫ����App: 
// �йش����ʵ�֣������ ��ȫ����.cpp
//

class C��ȫ����App : public CWinApp
{
public:
	C��ȫ����App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C��ȫ����App theApp;