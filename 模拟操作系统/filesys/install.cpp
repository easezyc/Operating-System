#include <stdio.h>
#include <string.h>
#include "filesys.h"
//进入文件系统函数
void install(){
	int i,j;

	/* 1. read the filsys from the superblock*/   //xiao ???
	memcpy(&filsys,disk+BLOCKSIZ,sizeof(struct filsys));//超级块的内容拷贝到filsys中

	/* 2. initialize the inode hash chain*/
	for (i=0; i<NHINO; i++){		//将哈希队列初始化
		hinode[i].i_forw = NULL;
	}

	/*3. initialize the sys_ofile*/
	for (i=0; i<SYSOPENFILE; i++){	//对文件进行初始化，sys_ofile是文件数组
		sys_ofile[i].f_count = 0;
		sys_ofile[i].f_inode = NULL;
	}

	/*4. initialize the user*/
	for (i=0; i<USERNUM; i++){	//对用户数组进行初始化
		user[i].u_uid = 0;
		user[i].u_gid = 0;
		for (j=0; j<NOFILE; j++)//NOFILE表示用户最多打开的文件数
			user[i].u_ofile[j] = SYSOPENFILE+1;//初始化的用户的每个文件表所对应的文件数=最大文件数+1
	}

	/*5. read the main directory to initialize the dir*/
	cur_path_inode = iget(1);//获取磁盘结点号是1的内存结点
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);//目录的大小=当前内存结点的文件大小/目录名的长度（+4是因为int长度为4）

	for (i=0; i<DIRNUM; i++){		//初始化目录
		strcpy(dir.direct[i].d_name,"             ");
		dir.direct[i].d_ino = 0;
	}
 
	for (i=0; i<dir.size/(BLOCKSIZ/(DIRSIZ+4)); i++){ 
		memcpy(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i], 
		disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);//把当前内存结点i所指向的物理结点在磁盘里的内容拷贝到目录中
	}

	memcpy(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i], //目录保存对应的物理索引结点
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

