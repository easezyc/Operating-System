#ifndef _FILESYS_H
#define _FILESYS_H

#define delete 		dele     		//delete ��c++���Ǳ�����  by tangfl
//#define exit(a)		return			//����ʹ�� return         by tangfl

//All Defines
#define BLOCKSIZ        512			//ÿ����Ĵ�С
#define SYSOPENFILE     40
#define DIRNUM          128			//һ��Ŀ¼�� ��� ��Ŀ¼��
#define DIRSIZ          12			//Ŀ¼������ windows32��int����Ϊ4 xiao 14->12
#define PWDSIZ 			12			//������󳤶�
#define PWDNUM 			32
#define NOFILE   		20			//�û������ļ���
#define NADDR  			10			//i�ڵ��ڴ洢
#define NHINO 			128			//hash��  ?????????/* must be power of 2 */
#define USERNUM 		10
#define DINODESIZ 		52			//?????? int=4 �ʶ���2*NADRR ��Դ������Ϊlong������short Ӧ��Ϊ50�ֽڣ��˴�����ϵͳΪ�˶����ڴ棬����˵���xiao 32->52


/*filesys*/
#define DINODEBLK  		32							//i�ڵ�ռ�õĿ���
#define FILEBLK   		512							//���ݿ���
#define NICFREE  		50							//�������ڿ��п��ջ��С
#define NICINOD  		50							//�������ڿ���i�ڵ������С
#define DINODESTART  	(2*BLOCKSIZ)				//i�ڵ㿪ʼ��ַ �ճ�1024����һ��Ϊ���������ڶ���Ϊ������
#define DATASTART  		((2+DINODEBLK)*BLOCKSIZ)	//��������ʼ��ַ DINODESTART+DINODEBLK*BLOCKSIZ	/*d:17408    0x4400*/


/*di_mode*/
#define DIEMPTY   		00000						//��Ȩ��
#define DIFILE      	01000						//���� �ļ�
#define DIDIR     		02000						//���� Ŀ¼

#define UDIREAD 		00001						//�û�Ȩ��
#define UDIWRITE  		00002
#define UDIEXICUTE  	00004
#define GDIREAD   		00010						//�û���Ȩ��
#define GDIWRITE  		00020
#define GDIEXICUTE  	00040
#define ODIREAD  		00100						//pubilcȨ��
#define ODIWRITE 		00200
#define ODIEXICUTE 		00400


#define READ  			1
#define WRITE 			2
#define EXICUTE 		3


#define DEFAULTMODE 	00777					//Ĭ��Ȩ��


/* i_flag */
#define  IUPDATE  		00002

/* s_fmod */
#define SUPDATE  		00001 

/* f_flag */
#define FREAD   		00001    //ֻ����
#define FWRITE   		00002    //��д��
#define FAPPEND   		00004    //��������


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
	struct inode  	*i_forw;// ָ���Ĺ�ϣ���к�����Ӧ�ĵ�ָ��
	struct inode  	*i_back;
	char 			i_flag; // i�ڵ�ĸ��±�־
	unsigned int  	i_ino;          /*����i �ڵ��־*/
	unsigned int  	i_count;     	/*���ü���*/
	unsigned short  di_number; 		/*�����ļ�������Ϊ0 ʱ����ɾ�����ļ�*/
	unsigned short  di_mode;  		/*��ȡȨ��*/
	unsigned short  di_uid;
	unsigned short  di_gid;
	unsigned short  di_size;   		/*�ļ���С*/
	unsigned int   	di_addr[NADDR];   /*������*/
};

struct dinode{
	unsigned short 	di_number; 		/*�����ļ���*/
	unsigned short 	di_mode; 		/*��ȡȨ��*/
	unsigned short 	di_uid;
	unsigned short 	di_gid;
	unsigned short 	di_size;  		/*�ļ���С*/
	unsigned int 	di_addr[NADDR];   /*������*/
};


struct direct{
	char d_name[DIRSIZ];  //g���ڴ��Ҫ������ļ���
	unsigned int d_ino;   //g����ǣ�0��ʾδ���䣬����ֵ��ʾ�ڵ���  / Ŀ¼��������ڵ���
};

struct filsys{
	unsigned short  	s_isize;   			/*i�ڵ�����*/
	unsigned long   	s_fsize;   			/*���ݿ����*/
	unsigned int   		s_nfree;    		/*���п�*/
	unsigned short  	s_pfree;  			/*���п�ָ��*/
	unsigned int  		s_free[NICFREE];  	/*���п��ջ*/
	
	unsigned int  		s_ninode;  			/*number of free inode in s_inode*///�������п���i�ڵ����
	short int 			s_pinode;  			/*pointer of the sinode*/
	unsigned int  		s_inode[NICINOD];   /*����i�ڵ�����*/
	unsigned int 		s_rinode;    		/*remember inode*/

	char 				s_fmod;  			/*�������޸ı�־*/
	};

struct pwd{
	unsigned short 		p_uid;
	unsigned short 		p_gid;
	char 				password [PWDSIZ];
	char				p_name[20];
};

struct dir{
	struct direct 		direct[DIRNUM];
	int 				size;  				/*��ǰĿ¼��С*/
};

struct hinode{
	struct inode 		*i_forw;   /*HASG��ָ��*/
};

struct file{
	char 			f_flag;    		/*�ļ�������־*/
	unsigned int 	f_count;  		/*���ü���*/
	struct inode 	*f_inode; 		/*ָ���ڴ�i�ڵ�*/
	unsigned long 	f_off;   		/*read/write character pointer����дλ���*/
};

struct user{
	unsigned short 	u_default_mode;
	unsigned short 	u_uid;             //    �ж��Ƿ��Ѿ���½
	unsigned short 	u_gid;
	char 			password[PWDSIZ];
	char			u_name[20];
	unsigned short 	u_ofile[NOFILE];   /*�û����ļ���*/
};
	
//all variables
extern struct list*	   namelist;
extern struct hinode   hinode[NHINO];
extern struct dir      dir;           /*��ǰĿ¼(���ڴ���ȫ������)*/
extern struct file     sys_ofile[SYSOPENFILE]; //��ǰϵͳ�洢�ļ���
extern struct filsys   filsys;        /*�ڴ��еĳ�����*/
extern struct pwd      pwd[PWDNUM];
extern struct user     user[USERNUM];
//extern struct file     *fd;           /*the file system column of all the system*/    //xiao
extern struct inode    *cur_path_inode;
extern int             user_id;
extern char            disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];
extern int			   usernum;
// all functions

extern struct        inode *iget(unsigned int);/* ���ã�	Ϊ����i�������Ӧ���ڴ�i���	 ����:	������Ĵ���i����	 ����ֵ��	ָ���Ӧ���ڴ�i����ָ��		*/
extern void          iput(struct inode *);/* ���ã�	�����ڴ�i��� ����:	ָ������յ��ڴ�i���ָ��	 ����ֵ��	��	*/
extern unsigned int  balloc(unsigned int);
extern unsigned int  balloc();
//������balloc ���ܣ�ά���������еĿ�������ջ������������ݿ飬���������� ���ز���������Ŀ��п��
extern void          bfree(unsigned int); //������bfree ���ܣ����ͷŵ��̿��ѹ����п��ջ�� ���������block_num���ͷŵ��̿��
extern struct inode* ialloc();//������ialloc ���ܣ��������i�ڵ㣬������Ӧ���ڴ�i�ڵ�ָ��
extern void          ifree(unsigned int);//�ͷų������е�i�ڵ�
extern int			 namei(char *);  //namei������������Ҫ���ҵ��ļ����������ļ���Ӧ���ڴ�Ŀ¼����i���������-1��ʾû�и��ļ������ܣ������ļ��ڵ�ǰĿ¼�¶�Ӧ���ڴ�Ŀ¼��ĺţ�
extern unsigned short iname(char *); //iname���������ܣ�������п�Ŀ¼�Ϊ��ǰ�ļ�����Ŀ¼���������Ҫ������ļ��������أ�0û�п���Ŀ¼�i�����Ŀ¼��š�
extern unsigned int  access(unsigned int, struct inode *, unsigned short);
//������access �������û�ID�ţ��ڴ�ڵ㣻Ҫ�жϵ�Ȩ�� ���ܣ�ͨ���û�Ȩ���ж��û��Ƿ��ܶԸ��ļ�������Ӧ�Ĳ�����Ȩ�޹�����1��Ȩ�޲�������0��
extern void          _dir(); //������dir ���ܣ���Ŀ¼ ���⣺����ʾ�ļ����Ե�һλ����ͨ�ļ�orĿ¼�ļ���
extern void          mkdir(char *);//������mkdir ���ܣ��½�Ŀ¼ ������Ŀ¼���� ���أ��� 
extern void          chdir(char *);//������chdir ���ܣ�cd����鿴Ŀ¼ ������Ŀ¼���� ���أ���
extern short		 open(int, char*, char);  //���ļ� user_id���û���ţ�*filename���ļ����֣�char�����飩��openmode���û��Ƿ�ӵ�еĴ򿪸��ļ���Ȩ��
extern int           creat(unsigned int, char *, unsigned short);
//cteat ���ܣ������ļ��������ҿ�д�򸲸ǣ���������i�ڵ㣬���򿪸��ļ��������ļ�ָ�� �������û�ID��user_id �ļ�����filename ��ȡȨ�ޣ�mode ���⣺û�п���Ŀ¼���0ʱĿ¼��СҲ++��
extern unsigned int  read(int fd, char *buf, unsigned int size);/*���ܣ����ļ� ������fd�����û������ļ���ţ�*buf����������size���ļ���С ���أ��ļ��������ֵĴ�С*/
extern unsigned int  write(int fd, char *buf, unsigned int size);/*���ܣ�д�ļ� ������fd���ļ����û����ı�ţ�*buf����������size���ļ���Ҫ�Ĵ�С ���أ�������д����Ĵ�С*/
extern int           login(unsigned short, char *);//�û���¼����   �������û�id������
extern int           logout(unsigned short);//�û��˳���¼  �������û�id
extern void          install();//�����ļ�ϵͳ����
extern void          format(); //�ļ�ϵͳ��ʽ������
extern void          close(unsigned int, unsigned short);
//������close ���ܣ��رմ��ļ�ʱ���޸��û��򿪱��ϵͳ�򿪱��еļ�¼��Ϣ ���������user_id : �û�id cfd : �ļ�ָ�� 			   �Ľ����ļ�û�б���ʱ��ʾ
extern void          halt();//ֹͣ���� ���ļ�д�ش��̲��ͷ��û���ϵͳ�ļ�����Ϣ�ڵ�
extern void          delete(char *); //������delete ���ܣ�ɾ���ļ� �������ļ�����filename
extern int 			 shell(int user_id, char *str);//�����������
extern void          modpwd(int uid, char* newpwd);//�޸�����
extern void			 modgroup(int uid, int newgid);//�޸��û���
extern int			 adduser1(char* name);//����û�����ָ���û�id
extern int			 adduser2(char* name, int uid);//����û���ָ���û�id
extern int			 login2(char* name, char *passwd);//��¼�ĵڶ��ַ�ʽ�����û�����¼
extern void			 trace(int uid,char* name);//���������������ڼ�¼��ǰ·��
#endif



