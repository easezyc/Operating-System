#include <stdio.h>
#include <string.h>
#include "filesys.h"
/**********************************************************************************
������format
��������
���ܣ��ļ�ϵͳ��ʽ������
***********************************************************************************/
void format(){       //          ��ʼ������
	struct inode *inode;
	struct direct dir_buf[BLOCKSIZ/(DIRSIZ+4)];
	struct pwd passwd[32];
	unsigned int block_buf[BLOCKSIZ/sizeof(int)];
	int i,j;
	namelist = (struct list*)malloc(sizeof(struct list));
	strcpy(namelist->name, "/");
	namelist->next = NULL;
	//��ʼ��Ӳ��
	memset(disk, 0x00, ((DINODEBLK+FILEBLK+2)*BLOCKSIZ));

	/* ��ʼ���û����������û�������� */
	strcpy(passwd[0].p_name,"root");
	passwd[0].p_uid = 2116;
	passwd[0].p_gid = 03;
	strcpy(passwd[0].password, "dddd");

	strcpy(passwd[1].p_name, "user0");
	passwd[1].p_uid = 2117;
	passwd[1].p_gid = 03;
	strcpy(passwd[1].password, "bbbb");

	strcpy(passwd[2].p_name, "user1");
	passwd[2].p_uid = 2118;
	passwd[2].p_gid = 04;
	strcpy(passwd[2].password, "abcd");

	strcpy(passwd[3].p_name, "user2");
	passwd[3].p_uid = 2119;
	passwd[3].p_gid = 04;
	strcpy(passwd[3].password, "cccc");

	strcpy(passwd[4].p_name, "user3");
	passwd[4].p_uid = 2120;
	passwd[4].p_gid = 05;
	strcpy(passwd[4].password, "eeee");
	usernum = 5;
	/* 1.creat the main directory and its sub dir etc and the file password */
    /* ������Ŀ¼����Ŀ¼etc ���ļ�password */
	inode = iget(0);   /* 0 empty dinode id*///Ϊ����i�������Ӧ���ڴ�i���
	inode->di_number = 1;			//�����ļ���Ϊ1
	inode->di_mode = DIEMPTY;//���ô�ȡȨ��
	inode->di_mode = DEFAULTMODE | DIDIR;
	iput(inode);//�ͷŽڵ�

	inode = iget(1);   /* 1 main dir id*/
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;//����Ȩ������
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0; /*block 0# is used by the main directory*///�����0������Ŀ¼

	strcpy(dir_buf[0].d_name,"..");//��ʼ��Ŀ¼���Ŀ¼���������ڵ��
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 1;
	strcpy(dir_buf[2].d_name,"etc");
	dir_buf[2].d_ino = 2;
	strcpy(dir_buf[3].d_name, "home");
	dir_buf[3].d_ino = 4;

	memcpy(disk+DATASTART, &dir_buf, 3*(DIRSIZ+4));//����Ŀ¼������ǰ����ջ
	iput(inode);

	inode = iget(2);  /* 2  etc dir id *///��ʼ����Ŀ¼etc
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 1; /*block 1# is used by the etc directory*///��ʼ�������1�����etc��Ŀ¼

	strcpy(dir_buf[0].d_name,"..");//��ʼ��Ŀ¼��Ŀ¼���������ڵ��
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 2;
	strcpy(dir_buf[2].d_name,"password");
	dir_buf[2].d_ino = 3;

	memcpy(disk+DATASTART+BLOCKSIZ*1, dir_buf, 3*(DIRSIZ+4));//�������ڶ���
	iput(inode);

	inode = iget(3);  /* 3  password id *///��ʼ���ļ�password
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ; //��ʼ����СΪһ�������Ĵ�С
	inode->di_addr[0] = 2; /*block 2# is used by the password file*///��ʼ�������2������ļ�password

	for (i=5; i<PWDNUM; i++){//��5�Ժ��û������ʼ�����û����û����Ϊ0������Ϊ��
		passwd[i].p_uid = 0;
		passwd[i].p_gid = 0;
		strcpy(passwd[i].password, "            ");  // PWDSIZ " "
	}

	memcpy(pwd, passwd, 32*sizeof(struct pwd));//������������
	memcpy(disk+DATASTART+BLOCKSIZ*2, passwd, BLOCKSIZ);
	iput(inode);

	/*2. initialize the superblock *///��ʼ�������飬�����ļ�ϵͳ�ĸ�������

	filsys.s_isize = DINODEBLK;
	filsys.s_fsize = FILEBLK;

	filsys.s_ninode = DINODEBLK * BLOCKSIZ/DINODESIZ - 4;
	filsys.s_nfree = FILEBLK - 3;//���ݿ�����ȥ��ʼ��ʱ�ֳ�ȥ������

	for (i=0; i < NICINOD; i++){
		/* begin with 4,    0,1,2,3, is used by main,etc,password */
		filsys.s_inode[i] = 4+i;//��4��ʼ����ڵ��
	}

	filsys.s_pinode = 0;
	filsys.s_rinode = NICINOD + 4;

	block_buf[NICFREE-1] = FILEBLK+1;  /*FILEBLK+1 is a flag of end*/
	for (i=0; i<NICFREE-1; i++)
		block_buf[NICFREE-2-i] = FILEBLK-i-1;			//�����һ�����ݿ鿪ʼ����

	memcpy(disk+DATASTART+BLOCKSIZ*(FILEBLK-NICFREE), block_buf, BLOCKSIZ);//����һ����
	for (i=FILEBLK-2*NICFREE+1; i>2; i-=NICFREE){
		for (j=0; j<NICFREE;j++){//�Գ������ڿ��п��ջ��СΪ��λ
			block_buf[j] = i+j;
		}
		memcpy(disk+DATASTART+BLOCKSIZ*(i-1), block_buf, BLOCKSIZ);//����ÿһ����
	}
	i += NICFREE;
	j = 1;
	for (; i>3; i--)
	{
		filsys.s_free[NICFREE-j] = i-1; //��ʼ�����п��ջ
		j ++;
	}

	filsys.s_pfree = NICFREE - j+1; //��ʼ�����п�ָ��
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));//��������
	return;

}
