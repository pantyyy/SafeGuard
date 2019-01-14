// Thread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ����.h"
#include "Thread.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <TlHelp32.h>

// CThread �Ի���

IMPLEMENT_DYNAMIC(CThread, CDialogEx)

CThread::CThread(DWORD pid, CWnd* pParent /*=NULL*/)
	: CDialogEx(CThread::IDD, pParent)
{
	nPid = pid;
}


CThread::~CThread()
{
}

void CThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Thread);
}


BEGIN_MESSAGE_MAP(CThread, CDialogEx)
END_MESSAGE_MAP()


// CThread ��Ϣ�������


BOOL CThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���öԻ�����
	CString DlgTitle("�߳���Ϣ");
	SetWindowText(DlgTitle.GetBuffer());

	m_Thread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Thread.InsertColumn(0, L"�߳�ID", 0, 80);
	m_Thread.InsertColumn(1, L"����ID", 0, 80);
	m_Thread.InsertColumn(2, L"Ĭ�ϵ����ȼ�", 0, 150);

	// 2. ����һ���߳̿���
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);

	// 3. ����һ���ṹ�����ڱ����̵߳���Ϣ
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	int nCount = 0;
	// 4. ���Դӿ����л�ȡ����һ���߳�
	if (Thread32First(Snapshot, &ThreadInfo))
	{
		// 5. �Ա����������ݽ������
		do {
			// ������������̸߳�����ΪPID�ͽ������
			if (ThreadInfo.th32OwnerProcessID == nPid)
			{
				m_Thread.InsertItem(nCount, L"");

				// �̵߳�ID
				CString t_id;
				t_id.Format(TEXT("%d") , ThreadInfo.th32ThreadID);
				m_Thread.SetItemText(nCount, 0 , t_id);

				// ���̵�ID
				CString p_id;
				p_id.Format(TEXT("%d") , nPid);
				m_Thread.SetItemText(nCount, 1, p_id);

				// Ĭ�ϵ����ȼ������ȼ�Խ�ߣ����ȵ�Խ��
				CString priority;
				priority.Format(TEXT("%d"), ThreadInfo.tpBasePri);
				m_Thread.SetItemText(nCount, 2, priority);

				nCount++;
			}
			//6. ���Ի�ȡ��һ���̵߳���Ϣ
		} while (Thread32Next(Snapshot, &ThreadInfo));
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
