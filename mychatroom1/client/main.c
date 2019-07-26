/*************************************************************************
	> File Name: main.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2019年07月26日 12时40分36秒 CST
 ************************************************************************/

#include<stdio.h>
#include"net.h"


struct message{

	int action;
	char msg[1024];
	char toname[20];
};

void *recv_message(void * arg)
{
	int sfd=*(int *)arg;
	struct message *msg=(struct message *)malloc(sizeof(struct message));
	 int ret;
	while(1)
	{
		ret=recv(sfd,msg,sizeof(struct message),0);
		if(-1==ret)
		{
			perror("send");
			exit(1);
		}


		switch(msg->action)
		{
			case 1:
			{
				printf("reg success\n");
			}break;

			case 2:
			{
				printf("recv:%s\n",msg->msg);
			}break;

			case 3:
			{
				printf("all recv:%s\n",msg->msg);
			}break;


		}

		memset(msg,0,sizeof(struct message));
		usleep(5);
		}
		close(sfd);
}

int main()
{
	int sfd;
	int ret;

	sfd=InitSocket();
	if(-1==sfd)
	{
		perror("socket");
		exit(1);
	}

	pthread_t pid;
	ret=pthread_create(&pid,NULL,recv_message,&sfd);
	if(-1==ret)
	{
		perror("pthread_creat");
		exit(1);
	}

	printf("cmd: reg  send  all\n");
	char cmd[20];
	char name[20];
	char toname[20];
	char message[1024];

	struct message *msg=(struct message *)malloc(sizeof(struct message));

	while(1)
	{
		printf("please input cmd\n");
		scanf("%s",cmd);

		if(strcmp(cmd,"reg")==0)
		{
			
			printf("please input your name\n");
			scanf("%s",name);
			msg->action=1;
			strcpy(msg->msg,name);
			ret=send(sfd,msg,sizeof(struct message),0);
			if(-1==ret)
			{
				perror("reg-send");
				exit(1);
			}
			usleep(10);
		}

		if(strcmp(cmd,"send")==0)
		{
			printf("please send toname\n");
			scanf("%s",toname);
			printf("please input send message\n");
			scanf("%s",message);
			msg->action=2;
			strcpy(msg->toname,toname);
			strcpy(msg->msg,message);

			
			ret=send(sfd,msg,sizeof(struct message),0);
			if(-1==ret)
			{
				perror("send-action2");
				exit(1);
			}

		}

		if(strcmp(cmd,"all")==0)
		{
			printf("please input send all essage\n");
			scanf("%s",message);
			msg->action=3;
			strcpy(msg->msg,message);

			
			ret=send(sfd,msg,sizeof(struct message),0);
			if(-1==ret)
			{
				perror("all-action3");
				exit(1);
			}

		}
	}
	close(sfd);
 
	return 0;
}
