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
    format();                 //      初始化操作
	install();//进入文件系统
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
		}//输入用户名和密码，用户登入
		if (user[user_id].u_uid != 2116)
		{
			chdir("home");
			chdir(user[user_id].u_name);//cd命令查看目录
		}
		do{
			printf("%s", user[user_id].u_name);
			printf("@ubuntu:");
			trace(user[user_id].u_uid, user[user_id].u_name);//输入命令提示处会根据用户类型不同位置不同而有不同的提示
			fflush(stdin);//清空输入缓存区
			gets(str);
		} while (shell(user_id, str));//输入命令
		logout(user_id);//用户登出
		printf("重新登录请输入Y\n请输入：");
		scanf("%c", &t);
	} while (t=='Y');
	halt();//停止函数 将文件写回磁盘并释放用户与系统文件和信息节点
	return 0;
}




