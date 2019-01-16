#pragma once
#include "afxsock.h"


enum msg_type{
	GetVirusMD5 = 1,
	UploadVirusMD5
};


#define BUFMSG	2048//一次接受消息的最大字节数
//数据包 , 描述消息的大小 , 类型 , 内容
struct Msg_Pack
{
	int nMsgSize;
	int nMsgType;
	char nMsgBuff[BUFMSG];
};




class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();

	//初始化客户端套接字，并连接服务器
	bool ConnectServer(char* szIp, WORD port);
	//发送消息 , 参数:消息内容 , 消息大小 , 消息类型
	bool SendMsg(char* data, int length, msg_type nType);
	void SendMsgByType(char* data, int length, msg_type type);

	//客户端套接字
	SOCKET m_sClient = NULL;
};

