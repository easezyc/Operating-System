#include <stdio.h>
#include "filesys.h"
/*打开文件
user_id：用户编号，*filename：文件名字（char型数组），openmode：用户是否拥有的打开该文件的权限
*/
short open(int user_id, char *filename,char openmode){		//返回值为short的函数
	unsigned int dinodeid;		//定义int类型的参数dinodeid
	struct inode *inode;		//定义inode结构类型的指针inode
	int i,j,k;			//定义三个int型参数i，j，k

	dinodeid = namei(filename);	//找到名字为filename的文件所在位置，并将其赋给dinodeid
	if (dinodeid == 0){		//没有找到名字为filename的文件
		printf("\nfile does not existed!!!\n");		//输出文件不存在的信息
		return -1;		//跳出open函数，并返回-1，表示出错，不能打开该文件
	}
	inode = iget(dir.direct[dinodeid].d_ino);
	if (!(inode->di_mode &DIFILE)){	//这个filename不是文件类型
		printf("%s is not a file!!!\n",filename);	//输出不是文件的信息
		iput(inode);		//回收该内存结点
		return -1;		//跳出open函数，返回-1，不能打开该文件
	}
	if (!access(user_id, inode, openmode)){		//通过用户权限（openmode）判断当前用户是否能对该文件进行相应的操作（open），如果没有该权限，进入if函数
		printf("\nfile open has not access!!!\n");	//当前用户没有打开文件的权限
		iput(inode);		//回收该内存节点
		return -1;		//跳出open函数，返回-1，不能打开该文件
	}

	for (i=1; i<SYSOPENFILE; i++){	//系统只能打开SYSOPENFILE个文件，对这些文件进行如下操作
		if (sys_ofile[i].f_count == 0)		//当前系统存储文件区中没有文件
			break;		//此时i记录的是符合上述条件的系统区的位置
	}

	if (i == SYSOPENFILE){		//只有在上述循环结束后没有找到空的系统存储文件区，才会进入该if函数
		printf("\nsystem open file too much\n");	//所有系统存储文件区都被占用，表示系统打开的文件太多
		iput(inode);		//回收该内存节点
		return -1;		//跳出open函数，返回-1，不能打开该文件
	}

	sys_ofile[i].f_inode = inode;	//将所需打开的文件放入空的系统存储文件区
	sys_ofile[i].f_flag = openmode;	//将该系统存储文件区的标志记为“可打开”的类型
	sys_ofile[i].f_count = 1;	//将该系统存储文件区中存的文件数记为1

	if (openmode & FAPPEND){	//如果该文件是FAPPEND（附加）类型
		sys_ofile[i].f_off = inode->di_size;		//将该文件的大小传给该系统存储文件区，用来进行读/写
	}else{
		sys_ofile[i].f_off = 0;	//不是FAPPEND类型，则该文件不能读写，没有大小
	}

	for (j=0; j<NOFILE; j++){	//用户最多可以打开NOFILE个文件，对打开的这些文件做以下操作
		if (user[user_id].u_ofile[j] == SYSOPENFILE + 1)	//当前用户打开文件区与上述系统存储文件区是一个
			break;		//跳出循环，此时j为符合上述条件的用户打开文件区的位置
	}

	if (j == NOFILE){		//只有在上述循环结束后没有在系统文件存储区找到用户想打开的文件，才会进入该if函数
		printf("\nuser open file too much!!!\n");	//所有系统存储文件区都被占用，表示系统打开的文件太多
		sys_ofile[i].f_count = 0;		//将系统存储文件区的文件数置0
		iput(inode);		//回收该内存节点
		return -1;		//跳出open函数，返回-1，不能打开该文件
	}

	user[user_id].u_ofile[j] = i;	//将该文件所在的系统存储文件区的位置传给当前用户打开文件区

	if(openmode & FWRITE){		//如果该文件的类型是FWRITE（可写）类型
		k=inode->di_size%BLOCKSIZ?1:0;		//文件大小正好占用整数个块，则k为0，否则k为1
		for (i=0; i<inode->di_size/BLOCKSIZ+k; i++)	//inode->di_size/BLOCKSIZ+k为文件所占用的块数，循环所有文件所占用的块
			bfree(inode->di_addr[i]);	//将当前所占用的物理块释放
		inode->di_size = 0;	//将文件大小置为0
	}
	return j;			//返回当前文件在用户打开文件区中的位置（编号）
}




