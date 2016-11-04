#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define MSGKEY 75

struct msgform
{
	long mtype;
	char mtext[256];
};
void client()
{
	int msgqid=msgget(MSGKEY,0777|IPC_CREAT);
	if(msgqid==-1)
	{
		printf("msgget failed with error.\n");
		exit(EXIT_FAILURE);
	}
	int i=10;
	for(;i>=1;i--)
	{
		struct msgform msg;
		msg.mtype=i;
		if(msgsnd(msgqid,&msg,256,0)==-1)
		{
			printf("msgsnd failed with error.\n");
			exit(EXIT_FAILURE);
		}
		else printf("%d(Client)send\n",msg.mtype);
		if(msgrcv(msgqid,&msg,256,30,0)==-1){
			printf("msgrcv failed with error.\n");
			exit(EXIT_FAILURE);
		}
		printf("(Client)recieved return\n");
	}
	exit(EXIT_SUCCESS);
}
void server()
{
	int msgqid=msgget(MSGKEY,0777|IPC_CREAT);
	if(msgqid==-1)
	{
		printf("msgget failed with error.\n");
		exit(EXIT_FAILURE);
	}
	while(1){
	struct msgform msg;
	if(msgrcv(msgqid,&msg,256,-11,0)==-1){
		printf("msgrcv failed with error.\n");
		exit(EXIT_FAILURE);
	}
	printf("%d(Server)received\n",msg.mtype);
	if(msg.mtype==1)exit(EXIT_SUCCESS);
	msg.mtype=30;
	if(msgsnd(msgqid,&msg,256,0)==-1){
		printf("msg return send failed");
		exit(EXIT_FAILURE);
	}
	}
}
int main()
{
	int pid;
	while((pid=fork())==-1);
	if(pid==0) client();
	else server();
}
