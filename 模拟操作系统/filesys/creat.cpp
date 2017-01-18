
#include <stdio.h>
#include "filesys.h"
/*********************************************************************
函数：creat
功能：创建文件，存在且可写则覆盖，否则申请i节点，并打开该文件，返回文件指针
参数：用户ID：user_id 文件名：filename 存取权限：mode
**********************************************************************/
int creat(unsigned int user_id, char *filename, unsigned short mode){
	struct inode *inode;                                              //创建磁盘结点结构体
	int dirid,di_ith;
	int i,j;

	dirid = namei(filename);//判断是否有同名文件/目录：返回文件对应的内存目录项标号i；如果返回-1表示没有该文件。
	if (dirid != -1){//如果存在同名文件/目录
		inode = iget(dir.direct[dirid].d_ino);//返回指向与内存目录项标号对应的节点的指针
		if(!(inode->di_mode&DIFILE)){//如果不是文件
			printf("命名冲突！\n");
		}
		if (access(user_id,inode,WRITE) == 0){//通过用户权限判断用户是否能对该文件进行写操作；权限够返回1，权限不够返回0。
			iput(inode);
			printf("\n creat access not allowed \n");//权限不够，不能对已存在文件进行写覆盖，退出程序，返回-1
			return -1;
		}
		j = inode->di_size%512?1:0;//同名文件大小是否是512的倍数，是赋值0，不是赋值1
		for (i=0; i<inode->di_size/BLOCKSIZ+j; i++)//总块数
			bfree(inode->di_addr[i]);//将涉及到的物理块清空加入空闲列表

		for (i=0; i<SYSOPENFILE; i++){
			if (sys_ofile[i].f_count != 0 && sys_ofile[i].f_inode == inode){
				sys_ofile[i].f_off = 0;//如果文件已经被打开且被引用，读写字符指针置零
			}
		}
		iput(inode);//回收指针
		return open(user_id,filename,WRITE);
	}else{
		inode = ialloc();//没有同名文件/目录
		di_ith = iname(filename);//分配目录项，没有空闲则返回0

		dir.size++;//目录大小加1
		dir.direct[di_ith].d_ino = inode->i_ino;//赋值
		inode->di_mode = mode;
		inode->di_uid = user[user_id].u_uid;
		inode->di_gid = user[user_id].u_gid;
		inode->di_size = 0;
		inode->di_number = 1;			//liwen change to 1
		iput(inode);//回收指针
		return open(user_id,filename,WRITE);//打开文件
	}
	return 0;
}








