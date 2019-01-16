#include "stdafx.h"
#include "ClientSocket.h"


CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}

bool CClientSocket::ConnectServer(char* szIp, WORD port)
{
	//��ʼ���׽��ֶ�̬��
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	//��鷵�صĿ�汾�Ƿ�2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//�����׽���
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}
	//���÷�������ַ
	SOCKADDR_IN servAddr = {};//��������ַ
	//����Э������
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(szIp);
	//���ַ����ĵ�ַת��Ϊ���ֽ�����
	inet_pton(AF_INET, szIp, &servAddr.sin_addr.S_un);
	//�������˿�
	servAddr.sin_port = htons((short)port);
	//���ӷ�����
	//SOCKADDR sService;//��������������׽���
	int nRetValue = connect(m_sClient, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR)
	{
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}
	return true;
}

bool CClientSocket::SendMsg(char* data, int length, msg_type nType)
{
	switch (nType)
	{
	case GetVirusMD5:
		SendMsgByType(data, length, nType);
		break;
	case UploadVirusMD5:
		SendMsgByType(data, length, nType);
		break;
	}

	return true;
}

void CClientSocket::SendMsgByType(char* data, int length, msg_type type)
{
	//����Ҫ���͵����ݰ�
	Msg_Pack msg_pack;
	//��������
	msg_pack.nMsgType = type;
	msg_pack.nMsgSize = length;
	strcpy_s(msg_pack.nMsgBuff, BUFMSG, data);

	send(m_sClient, (char*)&msg_pack, sizeof(msg_pack), NULL);
}
