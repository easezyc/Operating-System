#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h"

struct hinode 	hinode[NHINO];

struct dir 		dir;
struct file 	sys_ofile[SYSOPENFILE];
struct filsys 	filsys;
struct pwd 		pwd[PWDNUM];
struct user 	user[USERNUM];
struct inode* 	cur_path_inode;
struct list*	namelist;
int 			user_id;
char   			disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];
char 			str[100];
int				usernum;
int				rmmark;


int main(){
	int username;
	char password[16];
	rmmark = 0;
    format();                 //      ��ʼ������
	install();//�����ļ�ϵͳ
	user_id = -1;
	char* name = (char *)malloc(sizeof(char));
	printf("Welcome to mini filesystem!\n");
	char t='N';
	do{
		while (user_id == -1){
			printf("Login:");
			scanf("%s", name);
			username = atoi(name);
			printf("Password:");
			scanf("%s", password);
			if (username != 0)user_id = login(username, password);
			else user_id = login2(name, password);
		}//�����û��������룬�û�����
		if (user[user_id].u_uid != 2116)
		{
			chdir("home");
			chdir(user[user_id].u_name);//cd����鿴Ŀ¼
		}
		do{
			printf("%s", user[user_id].u_name);
			printf("@ubuntu:");
			trace(user[user_id].u_uid, user[user_id].u_name);//����������ʾ��������û����Ͳ�ͬλ�ò�ͬ���в�ͬ����ʾ
			fflush(stdin);//������뻺����
			gets(str);
		} while (shell(user_id, str));//��������
		logout(user_id);//�û��ǳ�
		printf("���µ�¼������Y\n�����룺");
		scanf("%c", &t);
	} while (t=='Y');
	halt();//ֹͣ���� ���ļ�д�ش��̲��ͷ��û���ϵͳ�ļ�����Ϣ�ڵ�
	return 0;
}




