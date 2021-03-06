#include "TinySocket.h"
#include <WINSOCK2.H>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

CTinySocket::CTinySocket()
{

}
void sk_startup(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
void sk_cleanup(void)
{
	//终止使用 DLL
	WSACleanup();
}
//socket获取图像
//sendline[] 发送指令
// length 期待获取的长度
// destip 相机ip
//destport 相机端口号
//返回：成功后返回1
//      失败后返回0
int CTinySocket::socket_com(char sendline[], int length, const char* destip, const int destport, char* buf)
{
	//socket connect
	sk_startup();
	char tempbuf[MAXBUFLINE];      //接收缓冲区
	int count = 0;             //接收总字节计数
	int rec_len, Ret;          //发送、接收状
	static struct sockaddr_in servaddr;
	//socket初始化
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET)
	{
		sk_cleanup();
		return -1;
		
	}
	else
	{
	  
	}
	//设置相机IP信息
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(destport);
	servaddr.sin_addr.s_addr = inet_addr(destip);
	//非阻塞模式
	u_long imode = 1;
	if (ioctlsocket(sockfd, FIONBIO, &imode) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		closesocket(sockfd);
		sk_cleanup();
		return -1;
	}
	struct timeval tm;
	tm.tv_sec = 5;
	tm.tv_usec = 0;
	//连接相机
	Ret = connect(sockfd, (SOCKADDR*)&servaddr, sizeof(servaddr));
	if (Ret == SOCKET_ERROR)
	{
		fd_set set;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		if (select(-1,NULL,&set,NULL,&tm) <= 0)
			return -1;
	}
	else
	{
		
	}

	//设置回阻塞
	u_long imodeb = 0;
	if (ioctlsocket(sockfd, FIONBIO, &imodeb) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		switch (err)
		{
		case WSANOTINITIALISED: qDebug() << "WSANOTINITIALISED"; break;
		case WSAENETDOWN: qDebug() << "WSAENETDOWN"; break;
		case WSAEINPROGRESS: qDebug() << "WSAEINPROGRESS"; break;
		case WSAENOTSOCK: qDebug() << "WSAENOTSOCK"; break;
		case WSAEFAULT:	qDebug() << "WSAEFAULT"; break;
		default:
			break;
		}
		return -1;
	}

	//发送采集命令
	Ret = send(sockfd, sendline, strlen(sendline), 0);
	if (Ret == SOCKET_ERROR)
	{
		return -1;
		
	}
	else
	{
		
	}
	int i2 = 0;
	//getDistanceSorted getDistanceAndAmplitudeSorted
	if ((strcmp(sendline,"getDistanceSorted")==0)||(strcmp(sendline, "getDistanceAndAmplitudeSorted") == 0))
	{
		//接收返回图像数据
		
		while (count < length) //153600 307200
		{
			rec_len = recv(sockfd, tempbuf, MAXBUFLINE, 0);
			for (int i = 0; i < rec_len; i++)
			{

				buf[i2] = tempbuf[i];
				i2++;

			}
			if (rec_len == SOCKET_ERROR)
			{
				
			}

			count = count + rec_len;

		}
	}
	else if (strcmp(sendline,"getTemperature")==0)
	{
		//获得温度
		rec_len = recv(sockfd, tempbuf, MAXBUFLINE, 0);
		for (int i = 0; i < rec_len; i++)
		{

			buf[i2] = tempbuf[i];
			i2++;

		}
		if (rec_len == SOCKET_ERROR)
		{
			return -1;
		}
		
	}
	else 
	{
		//其他指令
		rec_len = recv(sockfd, tempbuf, MAXBUFLINE, 0);
		/*qDebug() << "-------------------" << rec_len;
		qDebug() << "-------------------" << tempbuf;*/
		if (rec_len == SOCKET_ERROR)
		{
			return -1;
		}
		for (int i = 0; i < rec_len; i++)
		{
			buf[i] = tempbuf[i];
		}
	}
	
	//关闭套接字
	closesocket(sockfd);
	//终止使用 DLL
	sk_cleanup();
	if ((strcmp(sendline, "getDistanceSorted") == 0) || strcmp(sendline, "getDistanceAndAmplitudeSorted") == 0)
	{
		return 1;
	}
	else if (strcmp(sendline, "getTemperature") == 0)
	{
		return rec_len;
	}
	else
	{
		return 0;
	}
	
}
CTinySocket::~CTinySocket()
{
	

}
//获取深度数据
void CTinySocket::receive_depth()
{

}
//获取温度数据
void CTinySocket::receive_temperature()
{
}