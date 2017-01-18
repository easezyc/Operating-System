
#include <stdio.h>
#include "filesys.h"
/*********************************************************************
������creat
���ܣ������ļ��������ҿ�д�򸲸ǣ���������i�ڵ㣬���򿪸��ļ��������ļ�ָ��
�������û�ID��user_id �ļ�����filename ��ȡȨ�ޣ�mode
**********************************************************************/
int creat(unsigned int user_id, char *filename, unsigned short mode){
	struct inode *inode;                                              //�������̽��ṹ��
	int dirid,di_ith;
	int i,j;

	dirid = namei(filename);//�ж��Ƿ���ͬ���ļ�/Ŀ¼�������ļ���Ӧ���ڴ�Ŀ¼����i���������-1��ʾû�и��ļ���
	if (dirid != -1){//�������ͬ���ļ�/Ŀ¼
		inode = iget(dir.direct[dirid].d_ino);//����ָ�����ڴ�Ŀ¼���Ŷ�Ӧ�Ľڵ��ָ��
		if(!(inode->di_mode&DIFILE)){//��������ļ�
			printf("������ͻ��\n");
		}
		if (access(user_id,inode,WRITE) == 0){//ͨ���û�Ȩ���ж��û��Ƿ��ܶԸ��ļ�����д������Ȩ�޹�����1��Ȩ�޲�������0��
			iput(inode);
			printf("\n creat access not allowed \n");//Ȩ�޲��������ܶ��Ѵ����ļ�����д���ǣ��˳����򣬷���-1
			return -1;
		}
		j = inode->di_size%512?1:0;//ͬ���ļ���С�Ƿ���512�ı������Ǹ�ֵ0�����Ǹ�ֵ1
		for (i=0; i<inode->di_size/BLOCKSIZ+j; i++)//�ܿ���
			bfree(inode->di_addr[i]);//���漰�����������ռ�������б�

		for (i=0; i<SYSOPENFILE; i++){
			if (sys_ofile[i].f_count != 0 && sys_ofile[i].f_inode == inode){
				sys_ofile[i].f_off = 0;//����ļ��Ѿ������ұ����ã���д�ַ�ָ������
			}
		}
		iput(inode);//����ָ��
		return open(user_id,filename,WRITE);
	}else{
		inode = ialloc();//û��ͬ���ļ�/Ŀ¼
		di_ith = iname(filename);//����Ŀ¼�û�п����򷵻�0

		dir.size++;//Ŀ¼��С��1
		dir.direct[di_ith].d_ino = inode->i_ino;//��ֵ
		inode->di_mode = mode;
		inode->di_uid = user[user_id].u_uid;
		inode->di_gid = user[user_id].u_gid;
		inode->di_size = 0;
		inode->di_number = 1;			//liwen change to 1
		iput(inode);//����ָ��
		return open(user_id,filename,WRITE);//���ļ�
	}
	return 0;
}








