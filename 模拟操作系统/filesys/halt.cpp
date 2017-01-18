#include <stdio.h>
#include <string.h>
#include "filesys.h"

//停止函数 将文件写回磁盘并释放用户与系统文件和信息节点
void halt(){
	int i,j;

	/*1. write back the current dir */ //写回正确地址
	chdir("..");
	iput(cur_path_inode);

	/*2. free the u_ofile and sys_ofile and inode*///释放用户文件和系统文件和信息节点
	for (i = 0; i<USERNUM; i++){  //小于总用户数
		if(user[i].u_uid != 0){
			for (j = 0; j<NOFILE; j++){  //每个用户的文件数小于用户最多打开文件数
				if(user[i].u_ofile[j] != SYSOPENFILE + 1){//若该文件不等于系统打开的文件数+1 
					close(i,j);//关闭文件
					user[i].u_ofile[j] = SYSOPENFILE + 1;//令该文件等于系统打开的文件数+1
				}//if
			}//for
		}//if
	}//for

	/*3. write back the filesys to the disk*///将文件写回磁盘
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));//将超级块中的内容复制到磁盘

	/*4. close the file system column*/

	/*5. say GOOD BYE to all the user*/  //结束
	printf("\nGood Bye. See You Next Time. Please turn off the switch\n");
	exit(0);
}





























