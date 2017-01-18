#ifndef _FILESYS_H
#define _FILESYS_H

#define delete 		dele     		//delete 在c++里是保留字  by tangfl
//#define exit(a)		return			//建议使用 return         by tangfl

//All Defines
#define BLOCKSIZ        512			//每个块的大小
#define SYSOPENFILE     40
#define DIRNUM          128			//一个目录下 最多 子目录数
#define DIRSIZ          12			//目录名长度 windows32下int长度为4 xiao 14->12
#define PWDSIZ 			12			//密码最大长度
#define PWDNUM 			32
#define NOFILE   		20			//用户最多打开文件数
#define NADDR  			10			//i节点内存储
#define NHINO 			128			//hash数  ?????????/* must be power of 2 */
#define USERNUM 		10
#define DINODESIZ 		52			//?????? int=4 故多了2*NADRR 但源代码中为long，该做short 应该为50字节，此处可能系统为了对齐内存，故如此调整xiao 32->52


/*filesys*/
#define DINODEBLK  		32							//i节点占用的块数
#define FILEBLK   		512							//数据块数
#define NICFREE  		50							//超级块内空闲块堆栈大小
#define NICINOD  		50							//超级块内空闲i节点数组大小
#define DINODESTART  	(2*BLOCKSIZ)				//i节点开始地址 空出1024，第一个为引导区，第二块为超级块
#define DATASTART  		((2+DINODEBLK)*BLOCKSIZ)	//数据区开始地址 DINODESTART+DINODEBLK*BLOCKSIZ	/*d:17408    0x4400*/


/*di_mode*/
#define DIEMPTY   		00000						//空权限
#define DIFILE      	01000						//类型 文件
#define DIDIR     		02000						//类型 目录

#define UDIREAD 		00001						//用户权限
#define UDIWRITE  		00002
#define UDIEXICUTE  	00004
#define GDIREAD   		00010						//用户组权限
#define GDIWRITE  		00020
#define GDIEXICUTE  	00040
#define ODIREAD  		00100						//pubilc权限
#define ODIWRITE 		00200
#define ODIEXICUTE 		00400


#define READ  			1
#define WRITE 			2
#define EXICUTE 		3


#define DEFAULTMODE 	00777					//默认权限


/* i_flag */
#define  IUPDATE  		00002

/* s_fmod */
#define SUPDATE  		00001 

/* f_flag */
#define FREAD   		00001    //只读型
#define FWRITE   		00002    //可写型
#define FAPPEND   		00004    //附加类型


//Includes
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/* error */
#define DISKFULL  		65535

/* fseek origin */
//#define SEEK_SET  		0

struct list{
	char name[20];
	struct list* next;
};

struct inode{
	struct inode  	*i_forw;// 指定的哈希队列号所对应的的指针
	struct inode  	*i_back;
	char 			i_flag; // i节点的更新标志
	unsigned int  	i_ino;          /*磁盘i 节点标志*/
	unsigned int  	i_count;     	/*引用计数*/
	unsigned short  di_number; 		/*关联文件数。当为0 时，则删除该文件*/
	unsigned short  di_mode;  		/*存取权限*/
	unsigned short  di_uid;
	unsigned short  di_gid;
	unsigned short  di_size;   		/*文件大小*/
	unsigned int   	di_addr[NADDR];   /*物理块号*/
};

struct dinode{
	unsigned short 	di_number; 		/*关联文件数*/
	unsigned short 	di_mode; 		/*存取权限*/
	unsigned short 	di_uid;
	unsigned short 	di_gid;
	unsigned short 	di_size;  		/*文件大小*/
	unsigned int 	di_addr[NADDR];   /*物理块号*/
};


struct direct{
	char d_name[DIRSIZ];  //g用于存放要分配的文件名
	unsigned int d_ino;   //g结点标记，0表示未分配，其他值表示节点编号  / 目录项的索引节点数
};

struct filsys{
	unsigned short  	s_isize;   			/*i节点块块数*/
	unsigned long   	s_fsize;   			/*数据块块数*/
	unsigned int   		s_nfree;    		/*空闲块*/
	unsigned short  	s_pfree;  			/*空闲块指针*/
	unsigned int  		s_free[NICFREE];  	/*空闲块堆栈*/
	
	unsigned int  		s_ninode;  			/*number of free inode in s_inode*///超级块中空闲i节点个数
	short int 			s_pinode;  			/*pointer of the sinode*/
	unsigned int  		s_inode[NICINOD];   /*空闲i节点数组*/
	unsigned int 		s_rinode;    		/*remember inode*/

	char 				s_fmod;  			/*超级块修改标志*/
	};

struct pwd{
	unsigned short 		p_uid;
	unsigned short 		p_gid;
	char 				password [PWDSIZ];
	char				p_name[20];
};

struct dir{
	struct direct 		direct[DIRNUM];
	int 				size;  				/*当前目录大小*/
};

struct hinode{
	struct inode 		*i_forw;   /*HASG表指针*/
};

struct file{
	char 			f_flag;    		/*文件操作标志*/
	unsigned int 	f_count;  		/*引用计数*/
	struct inode 	*f_inode; 		/*指向内存i节点*/
	unsigned long 	f_off;   		/*read/write character pointer读、写位标记*/
};

struct user{
	unsigned short 	u_default_mode;
	unsigned short 	u_uid;             //    判断是否已经登陆
	unsigned short 	u_gid;
	char 			password[PWDSIZ];
	char			u_name[20];
	unsigned short 	u_ofile[NOFILE];   /*用户打开文件表*/
};
	
//all variables
extern struct list*	   namelist;
extern struct hinode   hinode[NHINO];
extern struct dir      dir;           /*当前目录(在内存中全部读入)*/
extern struct file     sys_ofile[SYSOPENFILE]; //当前系统存储文件区
extern struct filsys   filsys;        /*内存中的超级块*/
extern struct pwd      pwd[PWDNUM];
extern struct user     user[USERNUM];
//extern struct file     *fd;           /*the file system column of all the system*/    //xiao
extern struct inode    *cur_path_inode;
extern int             user_id;
extern char            disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];
extern int			   usernum;
// all functions

extern struct        inode *iget(unsigned int);/* 作用：	为磁盘i结点分配对应的内存i结点	 参数:	待分配的磁盘i结点号	 返回值：	指向对应的内存i结点的指针		*/
extern void          iput(struct inode *);/* 作用：	回收内存i结点 参数:	指向待回收的内存i结点指针	 返回值：	无	*/
extern unsigned int  balloc(unsigned int);
extern unsigned int  balloc();
//函数：balloc 功能：维护超级块中的空闲数据栈，分配空闲数据块，并返回其块号 返回参数：分配的空闲块号
extern void          bfree(unsigned int); //函数：bfree 功能：将释放的盘块号压入空闲块堆栈中 传入参数：block_num：释放的盘块号
extern struct inode* ialloc();//函数：ialloc 功能：分配磁盘i节点，返回相应的内存i节点指针
extern void          ifree(unsigned int);//释放超级块中的i节点
extern int			 namei(char *);  //namei函数：参数：要查找的文件名。返回文件对应的内存目录项标号i；如果返回-1表示没有该文件。功能：查找文件在当前目录下对应的内存目录项的号；
extern unsigned short iname(char *); //iname函数：功能：如果还有空目录项，为当前文件分配目录项；参数：将要分配的文件名。返回：0没有空闲目录项；i分配的目录项号。
extern unsigned int  access(unsigned int, struct inode *, unsigned short);
//函数：access 参数：用户ID号；内存节点；要判断的权限 功能：通过用户权限判断用户是否能对该文件进行相应的操作；权限够返回1，权限不够返回0。
extern void          _dir(); //函数：dir 功能：打开目录 问题：不显示文件属性第一位（普通文件or目录文件）
extern void          mkdir(char *);//函数：mkdir 功能：新建目录 参数：目录名字 返回：空 
extern void          chdir(char *);//函数：chdir 功能：cd命令查看目录 参数：目录名字 返回：空
extern short		 open(int, char*, char);  //打开文件 user_id：用户编号，*filename：文件名字（char型数组），openmode：用户是否拥有的打开该文件的权限
extern int           creat(unsigned int, char *, unsigned short);
//cteat 功能：创建文件，存在且可写则覆盖，否则申请i节点，并打开该文件，返回文件指针 参数：用户ID：user_id 文件名：filename 存取权限：mode 问题：没有空闲目录项返回0时目录大小也++了
extern unsigned int  read(int fd, char *buf, unsigned int size);/*功能：读文件 参数：fd：在用户区的文件编号，*buf：缓冲区，size：文件大小 返回：文件所读部分的大小*/
extern unsigned int  write(int fd, char *buf, unsigned int size);/*功能：写文件 参数：fd：文件在用户区的编号；*buf：缓冲区；size：文件需要的大小 返回：返回所写区域的大小*/
extern int           login(unsigned short, char *);//用户登录函数   参数是用户id和密码
extern int           logout(unsigned short);//用户退出登录  参数是用户id
extern void          install();//进入文件系统函数
extern void          format(); //文件系统格式化函数
extern void          close(unsigned int, unsigned short);
//函数：close 功能：关闭打开文件时，修改用户打开表和系统打开表中的记录信息 传入参数：user_id : 用户id cfd : 文件指针 			   改进：文件没有被打开时提示
extern void          halt();//停止函数 将文件写回磁盘并释放用户与系统文件和信息节点
extern void          delete(char *); //函数：delete 功能：删除文件 参数：文件名：filename
extern int 			 shell(int user_id, char *str);//输入相关命令
extern void          modpwd(int uid, char* newpwd);//修改密码
extern void			 modgroup(int uid, int newgid);//修改用户组
extern int			 adduser1(char* name);//添加用户，不指定用户id
extern int			 adduser2(char* name, int uid);//添加用户，指定用户id
extern int			 login2(char* name, char *passwd);//登录的第二种方式，用用户名登录
extern void			 trace(int uid,char* name);//遍历链表，链表用于记录当前路径
#endif



