#include <stdio.h>
#include "filesys.h"
/*���ļ�
user_id���û���ţ�*filename���ļ����֣�char�����飩��openmode���û��Ƿ�ӵ�еĴ򿪸��ļ���Ȩ��
*/
short open(int user_id, char *filename,char openmode){		//����ֵΪshort�ĺ���
	unsigned int dinodeid;		//����int���͵Ĳ���dinodeid
	struct inode *inode;		//����inode�ṹ���͵�ָ��inode
	int i,j,k;			//��������int�Ͳ���i��j��k

	dinodeid = namei(filename);	//�ҵ�����Ϊfilename���ļ�����λ�ã������丳��dinodeid
	if (dinodeid == 0){		//û���ҵ�����Ϊfilename���ļ�
		printf("\nfile does not existed!!!\n");		//����ļ������ڵ���Ϣ
		return -1;		//����open������������-1����ʾ�������ܴ򿪸��ļ�
	}
	inode = iget(dir.direct[dinodeid].d_ino);
	if (!(inode->di_mode &DIFILE)){	//���filename�����ļ�����
		printf("%s is not a file!!!\n",filename);	//��������ļ�����Ϣ
		iput(inode);		//���ո��ڴ���
		return -1;		//����open����������-1�����ܴ򿪸��ļ�
	}
	if (!access(user_id, inode, openmode)){		//ͨ���û�Ȩ�ޣ�openmode���жϵ�ǰ�û��Ƿ��ܶԸ��ļ�������Ӧ�Ĳ�����open�������û�и�Ȩ�ޣ�����if����
		printf("\nfile open has not access!!!\n");	//��ǰ�û�û�д��ļ���Ȩ��
		iput(inode);		//���ո��ڴ�ڵ�
		return -1;		//����open����������-1�����ܴ򿪸��ļ�
	}

	for (i=1; i<SYSOPENFILE; i++){	//ϵͳֻ�ܴ�SYSOPENFILE���ļ�������Щ�ļ��������²���
		if (sys_ofile[i].f_count == 0)		//��ǰϵͳ�洢�ļ�����û���ļ�
			break;		//��ʱi��¼���Ƿ�������������ϵͳ����λ��
	}

	if (i == SYSOPENFILE){		//ֻ��������ѭ��������û���ҵ��յ�ϵͳ�洢�ļ������Ż�����if����
		printf("\nsystem open file too much\n");	//����ϵͳ�洢�ļ�������ռ�ã���ʾϵͳ�򿪵��ļ�̫��
		iput(inode);		//���ո��ڴ�ڵ�
		return -1;		//����open����������-1�����ܴ򿪸��ļ�
	}

	sys_ofile[i].f_inode = inode;	//������򿪵��ļ�����յ�ϵͳ�洢�ļ���
	sys_ofile[i].f_flag = openmode;	//����ϵͳ�洢�ļ����ı�־��Ϊ���ɴ򿪡�������
	sys_ofile[i].f_count = 1;	//����ϵͳ�洢�ļ����д���ļ�����Ϊ1

	if (openmode & FAPPEND){	//������ļ���FAPPEND�����ӣ�����
		sys_ofile[i].f_off = inode->di_size;		//�����ļ��Ĵ�С������ϵͳ�洢�ļ������������ж�/д
	}else{
		sys_ofile[i].f_off = 0;	//����FAPPEND���ͣ�����ļ����ܶ�д��û�д�С
	}

	for (j=0; j<NOFILE; j++){	//�û������Դ�NOFILE���ļ����Դ򿪵���Щ�ļ������²���
		if (user[user_id].u_ofile[j] == SYSOPENFILE + 1)	//��ǰ�û����ļ���������ϵͳ�洢�ļ�����һ��
			break;		//����ѭ������ʱjΪ���������������û����ļ�����λ��
	}

	if (j == NOFILE){		//ֻ��������ѭ��������û����ϵͳ�ļ��洢���ҵ��û���򿪵��ļ����Ż�����if����
		printf("\nuser open file too much!!!\n");	//����ϵͳ�洢�ļ�������ռ�ã���ʾϵͳ�򿪵��ļ�̫��
		sys_ofile[i].f_count = 0;		//��ϵͳ�洢�ļ������ļ�����0
		iput(inode);		//���ո��ڴ�ڵ�
		return -1;		//����open����������-1�����ܴ򿪸��ļ�
	}

	user[user_id].u_ofile[j] = i;	//�����ļ����ڵ�ϵͳ�洢�ļ�����λ�ô�����ǰ�û����ļ���

	if(openmode & FWRITE){		//������ļ���������FWRITE����д������
		k=inode->di_size%BLOCKSIZ?1:0;		//�ļ���С����ռ���������飬��kΪ0������kΪ1
		for (i=0; i<inode->di_size/BLOCKSIZ+k; i++)	//inode->di_size/BLOCKSIZ+kΪ�ļ���ռ�õĿ�����ѭ�������ļ���ռ�õĿ�
			bfree(inode->di_addr[i]);	//����ǰ��ռ�õ�������ͷ�
		inode->di_size = 0;	//���ļ���С��Ϊ0
	}
	return j;			//���ص�ǰ�ļ����û����ļ����е�λ�ã���ţ�
}




