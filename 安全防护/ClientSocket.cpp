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
	//初始化套接字动态库
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	//检查返回的库版本是否2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//创建套接字
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}
	//设置服务器地址
	SOCKADDR_IN servAddr = {};//服务器地址
	//设置协议类型
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(szIp);
	//将字符串的地址转换为四字节整数
	inet_pton(AF_INET, szIp, &servAddr.sin_addr.S_un);
	//服务器端口
	servAddr.sin_port = htons((short)port);
	//连接服务器
	//SOCKADDR sService;//跟服务端相连的套接字
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
	//创建要发送的数据包
	Msg_Pack msg_pack;
	//填入数据
	msg_pack.nMsgType = type;
	msg_pack.nMsgSize = length;
	strcpy_s(msg_pack.nMsgBuff, BUFMSG, data);

	send(m_sClient, (char*)&msg_pack, sizeof(msg_pack), NULL);
}
