#include <stdio.h>
#include <string.h>
#include "filesys.h"
void modpwd(int uid,char* newpwd)//  ĞŞ¸ÄÃÜÂë
{
	int i;

	for (i = 0; i<PWDNUM; i++){
		if (uid == pwd[i].p_uid){
			strcpy(pwd[i].password,newpwd);
			break;
		}
	}
}
void modgroup(int uid, int newgid)//  ĞŞ¸ÄÃÜÂë
{
	int i;

	for (i = 0; i<PWDNUM; i++){
		if (uid == pwd[i].p_uid){
			pwd[i].p_gid=newgid;
			break;
		}
	}
}
int adduser1(char* name)
{
	int i, j;
	for (i = 0; i<usernum; i++){
		if (strcmp(name,pwd[i].p_name)==0){
			return 0;
		}
	}//if
	strcpy(pwd[usernum].p_name, name);
	pwd[usernum].p_uid = 2116+usernum;
	pwd[usernum].p_gid = 2116 + usernum;
	strcpy(pwd[usernum].password, "123");
	usernum++;
	return 1;
}
int adduser2(char* name,int uid)
{
	int i, j;
	for (i = 0; i<usernum; i++){
		if ((strcmp(name, pwd[i].p_name) == 0)||uid==pwd[i].p_uid){
			return 0;
		}
	}//if
	strcpy(pwd[usernum].p_name, name);
	pwd[usernum].p_uid = 2116 + usernum;
	pwd[usernum].p_gid = 2116 + usernum;
	strcpy(pwd[usernum].password, "123");
	usernum++;
	return 1;
}