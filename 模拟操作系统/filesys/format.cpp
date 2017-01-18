#include <stdio.h>
#include <string.h>
#include "filesys.h"
/**********************************************************************************
函数：format
参数：无
功能：文件系统格式化函数
***********************************************************************************/
void format(){       //          初始化操作
	struct inode *inode;
	struct direct dir_buf[BLOCKSIZ/(DIRSIZ+4)];
	struct pwd passwd[32];
	unsigned int block_buf[BLOCKSIZ/sizeof(int)];
	int i,j;
	namelist = (struct list*)malloc(sizeof(struct list));
	strcpy(namelist->name, "/");
	namelist->next = NULL;
	//初始化硬盘
	memset(disk, 0x00, ((DINODEBLK+FILEBLK+2)*BLOCKSIZ));

	/* 初始化用户名、所在用户组和密码 */
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
    /* 创建主目录和子目录etc 和文件password */
	inode = iget(0);   /* 0 empty dinode id*///为磁盘i结点分配对应的内存i结点
	inode->di_number = 1;			//关联文件数为1
	inode->di_mode = DIEMPTY;//设置存取权限
	inode->di_mode = DEFAULTMODE | DIDIR;
	iput(inode);//释放节点

	inode = iget(1);   /* 1 main dir id*/
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;//设置权限属性
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0; /*block 0# is used by the main directory*///物理块0用于主目录

	strcpy(dir_buf[0].d_name,"..");//初始化目录项的目录名和索引节点号
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 1;
	strcpy(dir_buf[2].d_name,"etc");
	dir_buf[2].d_ino = 2;
	strcpy(dir_buf[3].d_name, "home");
	dir_buf[3].d_ino = 4;

	memcpy(disk+DATASTART, &dir_buf, 3*(DIRSIZ+4));//拷贝目录项至当前数据栈
	iput(inode);

	inode = iget(2);  /* 2  etc dir id *///初始化子目录etc
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 1; /*block 1# is used by the etc directory*///初始化物理块1分配给etc子目录

	strcpy(dir_buf[0].d_name,"..");//初始化目录项目录名及索引节点号
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 2;
	strcpy(dir_buf[2].d_name,"password");
	dir_buf[2].d_ino = 3;

	memcpy(disk+DATASTART+BLOCKSIZ*1, dir_buf, 3*(DIRSIZ+4));//拷贝至第二块
	iput(inode);

	inode = iget(3);  /* 3  password id *///初始化文件password
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ; //初始化大小为一个物理块的大小
	inode->di_addr[0] = 2; /*block 2# is used by the password file*///初始化物理块2分配给文件password

	for (i=5; i<PWDNUM; i++){//从5以后用户密码初始化，用户及用户组皆为0，密码为空
		passwd[i].p_uid = 0;
		passwd[i].p_gid = 0;
		strcpy(passwd[i].password, "            ");  // PWDSIZ " "
	}

	memcpy(pwd, passwd, 32*sizeof(struct pwd));//拷贝至第三块
	memcpy(disk+DATASTART+BLOCKSIZ*2, passwd, BLOCKSIZ);
	iput(inode);

	/*2. initialize the superblock *///初始化超级块，保存文件系统的各项属性

	filsys.s_isize = DINODEBLK;
	filsys.s_fsize = FILEBLK;

	filsys.s_ninode = DINODEBLK * BLOCKSIZ/DINODESIZ - 4;
	filsys.s_nfree = FILEBLK - 3;//数据块数减去初始化时分出去的三个

	for (i=0; i < NICINOD; i++){
		/* begin with 4,    0,1,2,3, is used by main,etc,password */
		filsys.s_inode[i] = 4+i;//从4开始分配节点号
	}

	filsys.s_pinode = 0;
	filsys.s_rinode = NICINOD + 4;

	block_buf[NICFREE-1] = FILEBLK+1;  /*FILEBLK+1 is a flag of end*/
	for (i=0; i<NICFREE-1; i++)
		block_buf[NICFREE-2-i] = FILEBLK-i-1;			//从最后一个数据块开始分配

	memcpy(disk+DATASTART+BLOCKSIZ*(FILEBLK-NICFREE), block_buf, BLOCKSIZ);//读出一组块号
	for (i=FILEBLK-2*NICFREE+1; i>2; i-=NICFREE){
		for (j=0; j<NICFREE;j++){//以超级块内空闲块堆栈大小为单位
			block_buf[j] = i+j;
		}
		memcpy(disk+DATASTART+BLOCKSIZ*(i-1), block_buf, BLOCKSIZ);//读出每一组块号
	}
	i += NICFREE;
	j = 1;
	for (; i>3; i--)
	{
		filsys.s_free[NICFREE-j] = i-1; //初始化空闲块堆栈
		j ++;
	}

	filsys.s_pfree = NICFREE - j+1; //初始化空闲块指针
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));//读超级块
	return;

}
