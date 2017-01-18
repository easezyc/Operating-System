#include <stdio.h>
#include "filesys.h"
/*
功能：读文件
参数：fd：在用户区的文件编号，*buf：缓冲区，size：文件大小
返回：文件所读部分的大小
*/
unsigned int read(int fd, char *buf, unsigned int size){
	unsigned long off;			//定义long型参数off
	int block, block_off, i, j;		//定义四个int型参数
	struct inode *inode;			//定义inode结构型指针inode
	char *temp_buf;				//定义char型数组

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;		//将用户打开文件区的编号为fd的文件赋给inode
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD)){	//编号为fd的文件的标志不是FREAD（可读），进入if函数
		printf("\nthe file is not opened for read\n");		//输出不能读的信息
		return 0;			//跳出函数，返回
	}
	temp_buf = buf;				//将传入的缓冲区赋给temp_buf
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;		//将该文件的一个读/写标记位（尾指针？）赋给off
	if ((off+size) > inode->di_size){	//如果在文件实际大于在内存中占用的文件大小
		size = inode->di_size - off;	//将该文件在内存中的可读部分的大小赋给size
	}
	block_off = off % BLOCKSIZ;		//计算出文件不可读区域在内存块中的结束位置，为block_off
	block = off / BLOCKSIZ;			//计算出文件不可读区域占用的块数，为block
	if (block_off+size<BLOCKSIZ){		//如果剩余不可读部分与可读部分可以存在一个内存块中
		memcpy(buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, size);	//将内存块中的不可读区域复制到缓冲区中
		return size;			//返回可读部分占用内存的大小
	}
	memcpy(temp_buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,BLOCKSIZ-block_off);	//将最后一个存储不可读部分的内存块中剩余的大小复制到temp_buf
	temp_buf += BLOCKSIZ - block_off;	//给下面的for循环中的temp_buf一个初始大小
	j = (inode->di_size-off-block_off) / BLOCKSIZ;			//不可读部分占用的内存块的结束块号
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){		//对每个纯可读部分的内存块进行for操作，(size-(BLOCKSIZ-block_off))/BLOCKSIZ为可读部分开始的内存块号
		memcpy(temp_buf, disk+DATASTART+inode->di_addr[j+i]*BLOCKSIZ, BLOCKSIZ);		//将不可读部分的第i个内存块中的内容存入缓冲区
		temp_buf += BLOCKSIZ;		//扩大缓冲区的容量，以便存入下一块内存块
	}

	block_off = (size-(BLOCKSIZ-block_off))% BLOCKSIZ;		//文件不可读区域在内存中占用的不足一个物理块的大小放入block_off
	memcpy(temp_buf, disk+DATASTART+i*BLOCKSIZ, block_off);		//将所有不可读文件写入缓冲区
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;		//将所有部分都放入该文件对应的系统存储文件区中，并且将所有部分更改为不可读
	return size;		//返回所读部分的大小
}

/*
功能：写文件
参数：fd：文件在用户区的编号；*buf：缓冲区；size：文件需要的大小
返回：返回所写区域的大小
*/
unsigned int write(int fd, char *buf, unsigned int size){
	unsigned long off;			//定义long型参数off
	int block, block_off, i, j;		//定义四个int型参数
	struct inode *inode;			//定义inode结构类型的指针inode
	char *temp_buf;				//定义char型的数组temp_buf

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;			//将用户打开文件区的编号为fd的文件赋给inode
	if  (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE)){		//编号为fd的文件的标志不是FWRITE（可写），进入if函数
		printf("\nthe file is not opened for write\n");			//输出不能写的信息
		return 0;			//跳出函数，返回
	}
	//add by liwen to check the filesize and alloc the BLOCK
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;			//将该文件的一个可以开始读/写的标记位赋给off
	block = ((off+size)-inode->di_size)/BLOCKSIZ;				//在内存中尚需内存块的个数
	if(((off+size)-inode->di_size)%BLOCKSIZ)				//如果所需的的内存块不是整数
		block++;			//将所需内存块+1
	if(filsys.s_nfree < block){		//系统中的空闲块小于所需块
		printf("Not enough space to write so much bytes!\n");		//输出空间不够的错误
		return 0;			//跳出函数，返回
	}
	j = inode->di_size/BLOCKSIZ;		//j为文件占用内存块的个数
	if(inode->di_size % BLOCKSIZ){		//如果占的的内存块不是整数
		j ++;				//块数+1
	}
	if(j+block>NADDR){			//如果已占用块数与所需要块数比规定的内存中分配的块数多
		printf("To write so much bytes will exceed the file limit!!\n");//输出不被允许的错误
		return 0;			//跳出函数，返回
	}
	for(i=j;i<j+block;i++){			//从所需要的内存块开始执行到最后一个内存块
		inode->di_addr[i]=balloc();	//将空闲数据块的块号记为当前文件开始的位置
	}
	inode->di_size += size;			//整个文件全部的大小
	//end add
	temp_buf = buf;				//将缓冲区传给temp_buf

	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;			//将该文件的一个可以开始读/写的标记位赋给off
	block_off = off % BLOCKSIZ;		//计算出文件不可写区域在内存块中的结束位置
	block = off/BLOCKSIZ;			//计算出文件不可写区域占用的块数

	if (block_off+size<BLOCKSIZ){		//如果剩余不可写部分与可写部分可以存在一个内存块中
		memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, buf, size);	//将内存块中的可写（需要）区域复制到缓冲区中
		return size;			//返回可写部分占用内存的大小
	}
	memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,temp_buf,BLOCKSIZ-block_off);	//将缓冲区中的不够一个内存块的不可写部分存入磁盘中

	temp_buf += BLOCKSIZ-block_off;		//将缓冲区扩大
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){			//对每个纯可写部分的内存块进行for操作，(size-(BLOCKSIZ-block_off))/BLOCKSIZ为可写部分开始的内存块号
		memcpy(disk+DATASTART+inode->di_addr[block+1+i]*BLOCKSIZ, temp_buf, BLOCKSIZ);	//将缓冲区中的内存块放入需要进行写操作的位置
		temp_buf += BLOCKSIZ;		//扩大缓冲区的容量，以便存入下一块内存块
	}
	block_off = (size-(BLOCKSIZ-block_off)) % BLOCKSIZ;			//文件不可写区域在内存中占用的不足一个物理块的大小放入block_off
	memcpy(disk+DATASTART+block*BLOCKSIZ, temp_buf, block_off);		//将缓冲区中的内容放入需要写的地方
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;			//将所有部分都放入该文件对应的系统存储文件区中，并且将所有部分更改为不可写
	return size;				//返回所写区域的大小
}


