//用作服务器
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	char cSendBuf[100]={0};				//发送至客户端的字符串
	char cRecvBuf[100]={0};				//接收客户端的字符串
	char *ip;							//指向客户端的IP

	SOCKET sockServe;				//定义服务器套接字
	SOCKET sockClient;				//定义客户端套接字
	SOCKADDR_IN addrServe;			//定义服务端地址
	SOCKADDR_IN addrClient;			//定义客户端地址
	
	WSADATA wsaData;				//winsock结构体
	WORD wVersionRequested;			//版本
	
	
	int err;
	int len;
	int ch;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	
	if (err != 0)
	{
		printf("程序错误！");
		exit(1);
	}

	if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
	{
		printf("程序错误！");
		WSACleanup();
		exit(1);
	}

	//创建服务端套接字
	sockServe = socket(AF_INET, SOCK_STREAM, 0);

	//给服务端网络地址赋值
	addrServe.sin_family = AF_INET;
	addrServe.sin_port = htons(6000);
	addrServe.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//将网络地址与服务端口绑定
	bind(sockServe, (SOCKADDR*)&addrServe, sizeof(SOCKADDR));
	
	//启用监听
	listen(sockServe, 10);
	printf("服务器已启动！\n可以通话...\n");

	len = sizeof(SOCKADDR);
	sockClient = accept(sockServe, (SOCKADDR*)&addrClient, &len);
	ip = inet_ntoa(addrClient.sin_addr);
	printf("%s已连入", ip);
	while(1)
	{
		printf("%s:%s\n", ip, cRecvBuf);
		printf(">>");
		scanf("%s", cSendBuf);
		if (strcmp(cSendBuf, "exit") == 0)
		{
			send(sockClient, cSendBuf, strlen(cSendBuf)+1, 0);
			break;
		}
		send(sockClient, cSendBuf, strlen(cSendBuf)+1, 0);
		recv(sockClient, cRecvBuf, 100, 0);
	}
	closesocket(sockClient);	
}
