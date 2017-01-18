#include <stdio.h>
#include <string.h>
#include "filesys.h"
//使用用户id进行登录
int login(unsigned short uid, char *passwd){
	int i,j;

	for (i=0; i<PWDNUM; i++){
		if ((uid == pwd[i].p_uid) && (!strcmp(passwd, pwd[i].password))){	//如果输入的用户id和密码正确
			for (j=0; j<USERNUM; j++){
				if (user[j].u_uid == 0)		//如果用户名不在用户数组内，则跳出
					break;
			}
 
			if (j == USERNUM){
				printf("\ntoo much user in the systemm, waited to login\n");    //     表示同时在线的用户数量已经达到上限
				return -1;
			}else{
				user[j].u_uid = uid;            //     记录该用户用户id
				user[j].u_gid = pwd[i].p_gid;     //          记录该用户用户用户组
				user[j].u_default_mode = DEFAULTMODE;     //            记录用户权限
				strcpy(user[j].password, pwd[i].password);
				strcpy(user[j].u_name, pwd[i].p_name);
			}
			break;
		}//if
	}//for
	if (i == PWDNUM){
		printf("\nincorrect password\n");             //        密码错误
		return -1;
	}else{
		return j;
	}
}
//使用用户名进行登录
int login2(char* name, char *passwd){
	int i, j;

	for (i = 0; i<PWDNUM; i++){
		if (strcmp(name,pwd[i].p_name)==0 && (!strcmp(passwd, pwd[i].password))){
			for (j = 0; j<USERNUM; j++){
				if (user[j].u_uid == 0)
					break;
			}

			if (j == USERNUM){
				printf("\ntoo much user in the systemm, waited to login\n");    //     表示同时在线的用户数量已经达到上限
				return -1;
			}
			else{
				user[j].u_uid = pwd[i].p_uid;;            //     记录该用户用户id
				user[j].u_gid = pwd[i].p_gid;     //          记录该用户用户用户组
				user[j].u_default_mode = DEFAULTMODE;     //            记录用户权限
				strcpy(user[j].password, pwd[i].password);
				strcpy(user[j].u_name, pwd[i].p_name);
			}
			break;
		}//if
	}//for
	if (i == PWDNUM){
		printf("\nincorrect password\n");             //        密码错误
		return -1;
	}
	else{
		return j;
	}
}
//用户退出函数
//参数是用户id
int logout(unsigned short uid){
	int i,j,sys_no;
	struct inode *inode;

	for (i=0; i<USERNUM; i++){		//如果在user数组中找到了当前的用户id，就退出
		if (user[uid].u_uid == user[i].u_uid)//修改uid为user[uid].u_uid
			break;
	}

	if (i == USERNUM){		//已经都循环找了一遍还没找到就输出没有
		printf("\nno such a file\n");
		return 0;
	}

	for (j=0; j<NOFILE; j++){
		if (user[i].u_ofile[j] != SYSOPENFILE+1){		//如果用户打开的文件不等于它所拥有的初始化文件的数量
			/* iput the inode free the sys_ofile and clear the user_ofile*/
			sys_no = user[i].u_ofile[j];
			inode = sys_ofile[sys_no].f_inode;
			iput(inode);//释放结点
			sys_ofile[sys_no].f_count--;//文件引用的计数减1
			user[i].u_ofile[j] = SYSOPENFILE+1;
		}
	}
	user_id = -1;
	return 1;
}


































