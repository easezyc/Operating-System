
#include <stdio.h>
#include "filesys.h"

/******************************************************
函数：close
功能：关闭打开文件时，修改用户打开表和系统打开表中的纪录信息
*******************************************************/
void close(unsigned int user_id, unsigned short cfd){
	struct inode *inode;									//定义inode类型结构指针inode
	inode = sys_ofile[user[user_id].u_ofile[cfd]].f_inode;//inode指针指向该用户打开的文件
	if (inode)			//朱添加代码
		iput(inode);	//回收指针指向的内存i节点
	sys_ofile[user[user_id].u_ofile[cfd]].f_count -- ;	//系统打开表的文件引用计数-1
	user[user_id].u_ofile[cfd] = SYSOPENFILE+1;			//用户打开表中记录系统打开表置为SYSOPENFILE+1
	return;
}
