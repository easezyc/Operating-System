#include <stdio.h>
#include <string.h>
#include "filesys.h"

//ֹͣ���� ���ļ�д�ش��̲��ͷ��û���ϵͳ�ļ�����Ϣ�ڵ�
void halt(){
	int i,j;

	/*1. write back the current dir */ //д����ȷ��ַ
	chdir("..");
	iput(cur_path_inode);

	/*2. free the u_ofile and sys_ofile and inode*///�ͷ��û��ļ���ϵͳ�ļ�����Ϣ�ڵ�
	for (i = 0; i<USERNUM; i++){  //С�����û���
		if(user[i].u_uid != 0){
			for (j = 0; j<NOFILE; j++){  //ÿ���û����ļ���С���û������ļ���
				if(user[i].u_ofile[j] != SYSOPENFILE + 1){//�����ļ�������ϵͳ�򿪵��ļ���+1 
					close(i,j);//�ر��ļ�
					user[i].u_ofile[j] = SYSOPENFILE + 1;//����ļ�����ϵͳ�򿪵��ļ���+1
				}//if
			}//for
		}//if
	}//for

	/*3. write back the filesys to the disk*///���ļ�д�ش���
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));//���������е����ݸ��Ƶ�����

	/*4. close the file system column*/

	/*5. say GOOD BYE to all the user*/  //����
	printf("\nGood Bye. See You Next Time. Please turn off the switch\n");
	exit(0);
}





























