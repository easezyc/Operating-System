#include <stdio.h>
#include <string.h>
#include "filesys.h"
//ʹ���û�id���е�¼
int login(unsigned short uid, char *passwd){
	int i,j;

	for (i=0; i<PWDNUM; i++){
		if ((uid == pwd[i].p_uid) && (!strcmp(passwd, pwd[i].password))){	//���������û�id��������ȷ
			for (j=0; j<USERNUM; j++){
				if (user[j].u_uid == 0)		//����û��������û������ڣ�������
					break;
			}
 
			if (j == USERNUM){
				printf("\ntoo much user in the systemm, waited to login\n");    //     ��ʾͬʱ���ߵ��û������Ѿ��ﵽ����
				return -1;
			}else{
				user[j].u_uid = uid;            //     ��¼���û��û�id
				user[j].u_gid = pwd[i].p_gid;     //          ��¼���û��û��û���
				user[j].u_default_mode = DEFAULTMODE;     //            ��¼�û�Ȩ��
				strcpy(user[j].password, pwd[i].password);
				strcpy(user[j].u_name, pwd[i].p_name);
			}
			break;
		}//if
	}//for
	if (i == PWDNUM){
		printf("\nincorrect password\n");             //        �������
		return -1;
	}else{
		return j;
	}
}
//ʹ���û������е�¼
int login2(char* name, char *passwd){
	int i, j;

	for (i = 0; i<PWDNUM; i++){
		if (strcmp(name,pwd[i].p_name)==0 && (!strcmp(passwd, pwd[i].password))){
			for (j = 0; j<USERNUM; j++){
				if (user[j].u_uid == 0)
					break;
			}

			if (j == USERNUM){
				printf("\ntoo much user in the systemm, waited to login\n");    //     ��ʾͬʱ���ߵ��û������Ѿ��ﵽ����
				return -1;
			}
			else{
				user[j].u_uid = pwd[i].p_uid;;            //     ��¼���û��û�id
				user[j].u_gid = pwd[i].p_gid;     //          ��¼���û��û��û���
				user[j].u_default_mode = DEFAULTMODE;     //            ��¼�û�Ȩ��
				strcpy(user[j].password, pwd[i].password);
				strcpy(user[j].u_name, pwd[i].p_name);
			}
			break;
		}//if
	}//for
	if (i == PWDNUM){
		printf("\nincorrect password\n");             //        �������
		return -1;
	}
	else{
		return j;
	}
}
//�û��˳�����
//�������û�id
int logout(unsigned short uid){
	int i,j,sys_no;
	struct inode *inode;

	for (i=0; i<USERNUM; i++){		//�����user�������ҵ��˵�ǰ���û�id�����˳�
		if (user[uid].u_uid == user[i].u_uid)//�޸�uidΪuser[uid].u_uid
			break;
	}

	if (i == USERNUM){		//�Ѿ���ѭ������һ�黹û�ҵ������û��
		printf("\nno such a file\n");
		return 0;
	}

	for (j=0; j<NOFILE; j++){
		if (user[i].u_ofile[j] != SYSOPENFILE+1){		//����û��򿪵��ļ�����������ӵ�еĳ�ʼ���ļ�������
			/* iput the inode free the sys_ofile and clear the user_ofile*/
			sys_no = user[i].u_ofile[j];
			inode = sys_ofile[sys_no].f_inode;
			iput(inode);//�ͷŽ��
			sys_ofile[sys_no].f_count--;//�ļ����õļ�����1
			user[i].u_ofile[j] = SYSOPENFILE+1;
		}
	}
	user_id = -1;
	return 1;
}


































