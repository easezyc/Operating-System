#include <stdio.h>
#include <string.h>
#include "filesys.h"
/*********************************************************************
函数：dir
功能：打开目录

**********************************************************************/
void _dir(){
	unsigned int di_mode;
	int i,j,k;          //xiao
	struct inode *temp_inode;

	printf("\n CURRENT DIRECTORY :%s\n",dir.direct[0].d_name);
	printf("当前共有%d个文件/目录\n",dir.size);
	for (i=0; i<DIRNUM; i++){
		if (dir.direct[i].d_ino != DIEMPTY){//打印目录直到节点标志为零
			printf("%-14s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			di_mode = temp_inode->di_mode & 00777;	//九位文件属性
			for (j=0; j<9; j++){//从低位开始，为1则输出X，为0输出-，默认权限111111111-->xxxxxxxx
				if (di_mode%2){
					printf("x");
				}else{
					printf("-");
				}
				di_mode = di_mode/2;
			}
			printf("\ti_ino->%d\t",temp_inode->i_ino);//输出节点标志号
			if (temp_inode->di_mode & DIFILE){//如果是文件，则输出文件大小和块链接
				printf(" %ld ", temp_inode->di_size);
				printf("block chain:");
				j=(temp_inode->di_size%BLOCKSIZ)?1:0;//文件大小是否是块字节的整数倍，不是的话补齐
				for (k=0; k < temp_inode->di_size/BLOCKSIZ+j; k++)
					printf("%4d", temp_inode->di_addr[k]);//输出块号
				printf("\n");
			}else{
				printf("<dir>\n");//不是文件的话，输出<目录>字样
			}//else
			iput(temp_inode);//回收指针
		}// if (dir.direct[i].d_ino != DIEMPTY)
	}//for
	return;
}
/*********************************************************************
函数：mkdir
功能：新建目录
参数：目录名字
返回：空
**********************************************************************/
void mkdir(char *dirname){
	int dirid, dirpos;
	struct inode *inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	unsigned int block;

	dirid= namei(dirname);//返回对应的内存目录项标号
	if (dirid != -1){//存在同名目录/文件
		inode = iget(dirid);
		if (inode->di_mode & DIDIR) //存在同名目录
			printf("目录%s命名冲突！\n", dirname); //xiao
		else
			printf("%s是一个文件！\n", dirname);//存在同名文件
		iput(inode);//回收指针并退出
		return;
	}
	dirpos = iname(dirname);					//取得在addr中的空闲项位置,并将目录名写到此项里
	inode = ialloc();							//分配i节点
	dir.direct[dirpos].d_ino = inode->i_ino;	//设置该目录的磁盘i节点号
	dir.size++;									//目录数++

	strcpy(buf[0].d_name,"..");					//子目录的上一层目录 当前目录
	buf[0].d_ino = cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;				//子目录的本目录 子目录

	block = balloc();
	memcpy(disk+DATASTART+block*BLOCKSIZ, buf, BLOCKSIZ); //分配空闲数据块并返回块号
    //内存拷贝，从当前堆栈拷贝到指定位置一个块大小的内容
	inode->di_size = 2*(DIRSIZ+4);//设置大小
	inode->di_number = 1; //设置文件关联数
	inode->di_mode = user[user_id].u_default_mode|DIDIR;//设置权限
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;

	iput(inode);
	return;
}
/*********************************************************************
函数：chdir
功能：cd命令查看目录
参数：目录名字
返回：空
**********************************************************************/

void chdir(char *dirname){
	unsigned int dirid;
	struct inode *inode;
	unsigned short block;
	int i,j,low=0, high=0;

	dirid = namei(dirname);//查找目录项
	if (dirid == -1){
		printf("不存在目录%s！\n", dirname);
		return; //不存在目录并退出
	}
	inode =iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode&DIDIR)){
		printf("%s不是一个目录！\n",dirname);  //是文件不是目录，退出
		return;
	}
	for (i=0; i<dir.size; i++){ //pack当前目录
		if(dir.direct[i].d_ino == 0){
			for(j=DIRNUM-1;j>=0&&dir.direct[j].d_ino == 0;j--);//dirnum目录项的最大个数
			memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ+4);  //xiao
			dir.direct[j].d_ino = 0;
		}
	}
	j = cur_path_inode->di_size%BLOCKSIZ?1:0; //盘块数
	//写回当前目录
	for (i=0; i<cur_path_inode->di_size/BLOCKSIZ+j; i++){
		bfree(cur_path_inode->di_addr[i]);//将释放的盘块号压入空闲块堆栈中
	}
	for (i=0; i<dir.size; i+=BLOCKSIZ/(DIRSIZ+4)){
		block = balloc(); //分配物理块并返回块号
		cur_path_inode->di_addr[i] = block;
		memcpy(disk+DATASTART+block*BLOCKSIZ, &dir.direct[i], BLOCKSIZ);//访问节点，拷贝至相应地址
	}
	cur_path_inode->di_size = dir.size*(DIRSIZ+4);
	iput(cur_path_inode);//释放节点
	cur_path_inode = inode;//设置传入索引节点为当前路径节点

	j=0; //从磁盘中读并改变目录
	for (i=0; i<inode->di_size/BLOCKSIZ+1; i++){
		memcpy(&dir.direct[j],disk+DATASTART+inode->di_addr[i]*BLOCKSIZ, BLOCKSIZ); //将索引节点号写入当前目录项
		j+=BLOCKSIZ/(DIRSIZ+4);
	}
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);
	for (i=dir.size; i<DIRNUM; i++){
		dir.direct[i].d_ino = 0;//超出当前目录大小的目录项的索引节点数全部置零
	}
	if (strcmp(dirname, "..") == 0){//输出当前路径
		struct list* t= namelist;
		struct list* tt = namelist;
		tt = tt->next;
		if (tt != NULL)
		{
			while (tt->next != NULL)
			{
				tt = tt->next;
				t = t->next;
			}
			t->next = NULL;
		}
	}
	else if (strcmp(dirname, ".") == 0){

	}
	else{
		struct list* t = namelist;
		while (t->next != NULL)
		{
			t = t->next;
		}
		struct list* tt = (struct list*)malloc(sizeof(struct list));
		tt->next = NULL;
		strcpy(tt->name, dirname);
		t->next = tt;
	}
	//end by xiao

	return;
}
/*********************************************************************
函数：trace
功能：储存当前路径
参数：用户ID和名字
返回：空
**********************************************************************/
void trace(int uid,char* name)
{
	struct list* t = namelist;//新建结构体
	t = t->next;
	int mark = 0;
	while (t != NULL)
	{
		if (strcmp(t->name, "home") == 0 && t->next != NULL&&strcmp(t->next->name, name) == 0){
			mark = 1;//当前在根目录下
			break;
		}
		t = t->next;
	}
	t = namelist;
	t = t->next;
	if (uid == 2116)//超级用户   #
	{
		printf("/");
		while (t != NULL)
		{
			printf("%s", t->name);
			if (t->next != NULL)printf("/");
			t = t->next;//输出当前路径
		}
		printf("#");
	}
	else//普通用户到各自的目录下 ~$
	{
		if (mark == 1)
		{
			printf("~");
			int mark1 = 0;
			t = t->next;
			t = t->next;
			while (t != NULL)
			{
				if (mark1 == 0)
				{
					printf("/");
					mark1 = 1;
				}
				printf("%s", t->name);
				if (t->next != NULL)printf("/");
				t = t->next;
			}
			printf(" $");
		}
		else{
			printf("/");
			while (t != NULL)
			{
				printf("%s", t->name);
				if (t->next != NULL)printf("/");
				t = t->next;
			}
			printf(" $");
		}
	}
}









































