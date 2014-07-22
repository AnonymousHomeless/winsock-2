//用作客户端
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	char cSendBuf[1000];				//发送至客户端的字符串
	char cRecvBuf[1000];				//接收客户端的字符串
	char cmd[100];						//存储cmd命令

	SOCKET sockClient;				//定义客户端套接字
	SOCKADDR_IN addrServe;			//定义服务端地址
	
	SOCKADDR_IN addrClient;			//定义客户端地址
	WSADATA wsaData;				//winsock结构体
	WORD wVersionRequested;			//版本
	
	
	int err;
	int len;
	char ch;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	
	if (err != 0)
	{
		return 1;
	}

	if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
	{
		WSACleanup();
		return 1;
	}

	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	len = sizeof(SOCKADDR);
	addrServe.sin_family = AF_INET;
	addrServe.sin_port = htons(6000);
	addrServe.sin_addr.S_un.S_addr = inet_addr("10.28.20.133");
	
	connect(sockClient, (LPSOCKADDR)&addrServe, len);

	while(1)
	{
		//清空字符串
		ZeroMemory(cSendBuf, strlen(cSendBuf));
		ZeroMemory(cRecvBuf, strlen(cRecvBuf));
		recv(sockClient, cRecvBuf, 100, 0);
		printf("收到Serve命令：%s\n", cRecvBuf);
		
		if (strcmp(cRecvBuf, "test") == 0)
		{
			strcpy(cSendBuf, "服务端你好！");
		}
		else if(strcmp(cRecvBuf, "shutdown") == 0)
		{
			system("shutdown -s -t 1800");
			strcpy(cSendBuf, "已执行命令！30分钟后关机！");
		}
		else if(strcmp(cRecvBuf, "cancle") == 0)
		{
			system("shutdown -a");
			strcpy(cSendBuf, "已执行命令！关机已取消");
		}
		else if(strcmp(cRecvBuf, "exit") == 0)
		{
			break;
		}
		if (strlen(cRecvBuf) > 0)
		{
			send(sockClient, cSendBuf, strlen(cSendBuf)+1, 0);
		}
	}
	closesocket(sockClient);
	WSACleanup();
}

	
