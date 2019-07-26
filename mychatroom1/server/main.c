/*************************************************************************
	> File Name: main.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2019年07月26日 10时06分08秒 CST
 ************************************************************************/

#include<stdio.h>
#include"net.h"


struct message{
	int action;
	char msg[1024];
	char toname[20];
};

struct online {

	int cfd;
	char name[20];

	struct online *next;
};

struct online *head=NULL;

void insert_user(struct online * new)
{
	if(head==NULL)
	{
		new->next=NULL;
		head=new;
	}
	else
	{
		new->next=head->next;
		head->next=new;
	}
}

int findcfd(char *toname)
{
	if(head==NULL)
	{
		return -1;
	}
	struct online *temp=head;


	while(temp !=NULL)
	{
		if(strcmp(temp->name,toname)==0)
		{
			return temp->cfd;
		}
		temp=temp->next;
	}

	return -1;

}

void * Client_handler(void *arg)
{
	int ret;
	int n;
	int cfd=*(int *)arg;
	struct online *new;

	pthread_detach(pthread_self());
//	char buf[BUFSIZ];
	struct message *msg=(struct message *)malloc(sizeof(struct message));

	while(1)
	{
		memset(msg,0,sizeof(struct message));

		ret=recv(cfd,msg,sizeof(struct message),0);
		if(-1==ret)
		{
			perror("recv");
			exit(1);
		}

		if(ret==0)
		{
			printf("client %d offline\n",cfd);
			break;
		}

		switch(msg->action)
		{
			case 1:
			{
				printf("zhuce\n");
				new=(struct online *)malloc(sizeof(struct online));
				new->cfd=cfd;
				strcpy(new->name,msg->msg);

				insert_user(new);
				msg->action=1;
				send(cfd,msg,sizeof(struct message),0);
			}break;


			case 2:
			{
				int to_cfd;

				to_cfd=findcfd(msg->toname);

				msg->action=2;

				send(to_cfd,msg,sizeof(struct message),0);

			}break;

			case 3:
			{
				int i=0;
				struct online *temp=head;	
				while(temp != NULL)
				{
					i++;
					printf("%d\n",i);
					msg->action=3;
					ret=send(temp->cfd,msg,sizeof(struct message),0);
					if(-1==ret)
					{
						perror("seng-action3");
						exit(1);
					}

					temp=temp->next;
				}

			}break;

		}

	}
	close(cfd);


}
int main()
{
	int ret;
	int sfd,cfd;
	struct sockaddr_in clie_addr;
	sfd=InitSocket();

	printf("wait connect\n");

	while(1)
	{
		int len=sizeof(clie_addr);
		cfd=accept(sfd,(struct sockaddr *)&clie_addr,&len);
		if(-1==cfd)
		{
			perror("accept");
			exit(1);
		}

		printf("clie port is %d cfd is %d\n",ntohs(clie_addr.sin_port),cfd);
		
		pthread_t pid;
		ret=pthread_create(&pid,NULL,Client_handler,&cfd);
		if(-1==ret)
		{
			perror("pthread_creat");
			exit(1);
		}

		usleep(1000);
	}

	return 0;
}
