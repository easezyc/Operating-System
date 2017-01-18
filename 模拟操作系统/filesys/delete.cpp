#include <stdio.h>
#include "filesys.h"
/*********************************************************************
函数：delete
功能：删除文件
参数：文件名：filename
**********************************************************************/
void delete(char *filename){
	unsigned int dirid;
	struct inode *inode;

	dirid = namei(filename);//查找文件是否存在
	if (dirid == -1){
		printf("文件不存在，请检查!\n"); //文件不存在并退出
		return;
	}
	inode = iget(dir.direct[dirid].d_ino);//返回指向与内存目录项标号对应的节点的指针
	//inode->i_count--;
	if(!(inode->di_mode &DIFILE)){//对象不是文件
		printf("对象不是文件，请检查！\n");
		iput(inode);//回收结点并退出
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY; //指向与内存目录项标号对应的节点的指针置零
	dir.size --;//目录项大小-1
	inode->di_number--;//关联文件数-1
	iput(inode);//回收结点并退出
	return;
}











