#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h"

/* 函数名：	iget							*/
/* 作用：	为磁盘i结点分配对应的内存i结点	*/
/* 参数:	待分配的磁盘i结点号				*/
/* 返回值：	指向对应的内存i结点的指针		*/
struct inode * iget(unsigned int dinodeid) //内存inode的创建
{
	int existed = 0, inodeid;//内存节点所在的哈希队列号
	long addr;
	struct inode *temp, *newinode;


	int i;


	inodeid = dinodeid % NHINO;//计算内存结点应该在第几个哈希队列里，NHINO是哈希队列数
	if (hinode[inodeid].i_forw == NULL)//若该哈希队列为空，内存结点一定未被创建，hinode[]数组存的是哈希数组，hinode[inodeid].i_forw就是指哈希队列号是inodeid的指针
		existed = 0;
	else//若不为空，从该哈希队列头开始查找
	{
		temp = hinode[inodeid].i_forw; 
		while (temp)
		{
			if (temp->i_ino == dinodeid)//若找到，temp->i_ino指temp所指向的磁盘i的节点
			{
				existed = 1;
				temp->i_count ++;
				return temp;//返回该内存结点指针
			}
			else
				temp = temp->i_forw;

		}
	}

	/* 若没有找到 */   
	/* 1. 计算该磁盘i结点在文件卷中的位置 */
	addr = DINODESTART + dinodeid * DINODESIZ;//磁盘i节点开始地址+磁盘i*盘块大小

	/* 2. 分配一个内存i结点 */
	newinode = (struct inode *)malloc(sizeof(struct inode));//动态分配一个内存结点

	/* 3. 用磁盘i结点初始化内存i结点 */
	memcpy(&(newinode->di_number), disk+addr, DINODESIZ);

	/* 4. 将内存i结点链入相应的哈希队列里*/
	newinode->i_forw = hinode[inodeid].i_forw;//新内存结点的前向指针指向当前哈希队列指针所在的位置
	hinode[inodeid].i_forw = newinode; //哈希队列指针移到新结点的位置
	newinode->i_back = newinode;//新结点从尾部插入，所以后向指针i_back指向本身
	if (newinode->i_forw)//如果内存结点所在哈希队列的前面也存有结点
		newinode->i_forw->i_back = newinode;//前面结点的后向指针指向当前结点

	/*5. 初始化内存i结点的其他数据项 */
	newinode->i_count = 1;//引用计数为1
	newinode->i_flag = 0;   /* 表示未更新 */
	newinode->i_ino = dinodeid;//内存结点i对应的磁盘结点是dinodeid

	return newinode;
}

/* 函数名：	iput							*/
/* 作用：	回收内存i结点					*/
/* 参数:	指向待回收的内存i结点指针		*/
/* 返回值：	无								*/
void iput(struct inode *pinode)
{
	long addr;
	unsigned int block_num;
	int i;
	if (pinode->i_count > 1)//若引用计数>1
	{
		pinode->i_count --;
		return;
	}
	else
	{
		if (pinode->di_number != 0)//若联结计数不为0
		{
			/* 把内存i结点的内容写回磁盘i结点 */
			addr = DINODESTART + pinode->i_ino *DINODESIZ;//物理地址=初始地址+结点i*块大小
			memcpy(disk+addr, &pinode->di_number, DINODESIZ);//关联文件的内容拷入到磁盘里
		}
		else//关联文件数是0，删除节点和文件
		{
			/* 删除磁盘i结点和文件对应的物理块 */
			block_num = pinode->di_size/BLOCKSIZ;
			for (i = 0; i <= block_num; i++)
			{
				bfree(pinode->di_addr[i]);//释放物理块
			}
			if (pinode->di_size%BLOCKSIZ != 0){
				//bfree(pinode->di_addr[i]);
			}
			ifree(pinode->i_ino);//释放磁盘结点i
		}

		/* 释放内存i结点 */
		{
			int inodeid;
			inodeid = (pinode->i_ino)  % NHINO ;//找到所在的哈希队列

			/* 从该哈希队列里删除 */
			if (hinode[inodeid].i_forw == pinode)//如果当前结点在哈希队列的最尾部
			{
				hinode[inodeid].i_forw = pinode->i_forw;//让哈希队列当前指针指向pinode的前一个结点
				if (pinode->i_forw)//如果前一个结点存在
					pinode->i_forw->i_back = pinode->i_forw;//前一个结点的后置指针指向它自己
			}
			else//如果当前结点在哈希队列中间
			{
				pinode->i_back->i_forw = pinode->i_forw;//当前结点后向指针所指向的结点指向当前结点的前一个结点
				if (pinode->i_forw)//如果前一个结点存在
					pinode->i_forw->i_back = pinode->i_back;//它的后向指针指向当前结点的后一个结点
			}
		}
		free(pinode);
	}

	return;
}













