/*************************************************************************
	> File Name: net.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2019年07月26日 10时20分30秒 CST
 ************************************************************************/

#include<stdio.h>
#include"net.h"
#define PORT 8888
#define IP   "192.168.204.134"
int InitSocket()
{
	int ret;
	int sfd;
	struct sockaddr_in serv_addr;

	sfd=socket(PF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		exit(1);
	}

	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=PF_INET;
	serv_addr.sin_port=htons(PORT);
	serv_addr.sin_addr.s_addr=inet_addr(IP);
	ret=bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(-1==ret)
	{
		perror("bind");
		exit(1);
	}

	ret=listen(sfd,128);
	if(-1==ret)
	{
		perror("listen");
		exit(1);
	}

	return sfd;





}
