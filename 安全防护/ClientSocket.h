#pragma once
#include "afxsock.h"


enum msg_type{
	GetVirusMD5 = 1,
	UploadVirusMD5
};


#define BUFMSG	2048//һ�ν�����Ϣ������ֽ���
//���ݰ� , ������Ϣ�Ĵ�С , ���� , ����
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

	//��ʼ���ͻ����׽��֣������ӷ�����
	bool ConnectServer(char* szIp, WORD port);
	//������Ϣ , ����:��Ϣ���� , ��Ϣ��С , ��Ϣ����
	bool SendMsg(char* data, int length, msg_type nType);
	void SendMsgByType(char* data, int length, msg_type type);

	//�ͻ����׽���
	SOCKET m_sClient = NULL;
};

