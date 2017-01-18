#include <stdio.h>
#include <string.h>
#include "filesys.h"
//�����ļ�ϵͳ����
void install(){
	int i,j;

	/* 1. read the filsys from the superblock*/   //xiao ???
	memcpy(&filsys,disk+BLOCKSIZ,sizeof(struct filsys));//����������ݿ�����filsys��

	/* 2. initialize the inode hash chain*/
	for (i=0; i<NHINO; i++){		//����ϣ���г�ʼ��
		hinode[i].i_forw = NULL;
	}

	/*3. initialize the sys_ofile*/
	for (i=0; i<SYSOPENFILE; i++){	//���ļ����г�ʼ����sys_ofile���ļ�����
		sys_ofile[i].f_count = 0;
		sys_ofile[i].f_inode = NULL;
	}

	/*4. initialize the user*/
	for (i=0; i<USERNUM; i++){	//���û�������г�ʼ��
		user[i].u_uid = 0;
		user[i].u_gid = 0;
		for (j=0; j<NOFILE; j++)//NOFILE��ʾ�û����򿪵��ļ���
			user[i].u_ofile[j] = SYSOPENFILE+1;//��ʼ�����û���ÿ���ļ�������Ӧ���ļ���=����ļ���+1
	}

	/*5. read the main directory to initialize the dir*/
	cur_path_inode = iget(1);//��ȡ���̽�����1���ڴ���
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);//Ŀ¼�Ĵ�С=��ǰ�ڴ�����ļ���С/Ŀ¼���ĳ��ȣ�+4����Ϊint����Ϊ4��

	for (i=0; i<DIRNUM; i++){		//��ʼ��Ŀ¼
		strcpy(dir.direct[i].d_name,"             ");
		dir.direct[i].d_ino = 0;
	}
 
	for (i=0; i<dir.size/(BLOCKSIZ/(DIRSIZ+4)); i++){ 
		memcpy(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i], 
		disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);//�ѵ�ǰ�ڴ���i��ָ����������ڴ���������ݿ�����Ŀ¼��
	}

	memcpy(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i], //Ŀ¼�����Ӧ�������������
	disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);
	login(2116, "dddd");
	mkdir("home");
	chdir("home");
	mkdir("user0");
	mkdir("user1");
	mkdir("user2");
	mkdir("user3");
	chdir("..");
	logout(2116);
	return;	
}

