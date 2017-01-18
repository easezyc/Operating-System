#include <stdio.h>
#include "filesys.h"
/*
���ܣ����ļ�
������fd�����û������ļ���ţ�*buf����������size���ļ���С
���أ��ļ��������ֵĴ�С
*/
unsigned int read(int fd, char *buf, unsigned int size){
	unsigned long off;			//����long�Ͳ���off
	int block, block_off, i, j;		//�����ĸ�int�Ͳ���
	struct inode *inode;			//����inode�ṹ��ָ��inode
	char *temp_buf;				//����char������

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;		//���û����ļ����ı��Ϊfd���ļ�����inode
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD)){	//���Ϊfd���ļ��ı�־����FREAD���ɶ���������if����
		printf("\nthe file is not opened for read\n");		//������ܶ�����Ϣ
		return 0;			//��������������
	}
	temp_buf = buf;				//������Ļ���������temp_buf
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;		//�����ļ���һ����/д���λ��βָ�룿������off
	if ((off+size) > inode->di_size){	//������ļ�ʵ�ʴ������ڴ���ռ�õ��ļ���С
		size = inode->di_size - off;	//�����ļ����ڴ��еĿɶ����ֵĴ�С����size
	}
	block_off = off % BLOCKSIZ;		//������ļ����ɶ��������ڴ���еĽ���λ�ã�Ϊblock_off
	block = off / BLOCKSIZ;			//������ļ����ɶ�����ռ�õĿ�����Ϊblock
	if (block_off+size<BLOCKSIZ){		//���ʣ�಻�ɶ�������ɶ����ֿ��Դ���һ���ڴ����
		memcpy(buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, size);	//���ڴ���еĲ��ɶ������Ƶ���������
		return size;			//���ؿɶ�����ռ���ڴ�Ĵ�С
	}
	memcpy(temp_buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,BLOCKSIZ-block_off);	//�����һ���洢���ɶ����ֵ��ڴ����ʣ��Ĵ�С���Ƶ�temp_buf
	temp_buf += BLOCKSIZ - block_off;	//�������forѭ���е�temp_bufһ����ʼ��С
	j = (inode->di_size-off-block_off) / BLOCKSIZ;			//���ɶ�����ռ�õ��ڴ��Ľ������
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){		//��ÿ�����ɶ����ֵ��ڴ�����for������(size-(BLOCKSIZ-block_off))/BLOCKSIZΪ�ɶ����ֿ�ʼ���ڴ���
		memcpy(temp_buf, disk+DATASTART+inode->di_addr[j+i]*BLOCKSIZ, BLOCKSIZ);		//�����ɶ����ֵĵ�i���ڴ���е����ݴ��뻺����
		temp_buf += BLOCKSIZ;		//���󻺳������������Ա������һ���ڴ��
	}

	block_off = (size-(BLOCKSIZ-block_off))% BLOCKSIZ;		//�ļ����ɶ��������ڴ���ռ�õĲ���һ�������Ĵ�С����block_off
	memcpy(temp_buf, disk+DATASTART+i*BLOCKSIZ, block_off);		//�����в��ɶ��ļ�д�뻺����
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;		//�����в��ֶ�������ļ���Ӧ��ϵͳ�洢�ļ����У����ҽ����в��ָ���Ϊ���ɶ�
	return size;		//�����������ֵĴ�С
}

/*
���ܣ�д�ļ�
������fd���ļ����û����ı�ţ�*buf����������size���ļ���Ҫ�Ĵ�С
���أ�������д����Ĵ�С
*/
unsigned int write(int fd, char *buf, unsigned int size){
	unsigned long off;			//����long�Ͳ���off
	int block, block_off, i, j;		//�����ĸ�int�Ͳ���
	struct inode *inode;			//����inode�ṹ���͵�ָ��inode
	char *temp_buf;				//����char�͵�����temp_buf

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;			//���û����ļ����ı��Ϊfd���ļ�����inode
	if  (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE)){		//���Ϊfd���ļ��ı�־����FWRITE����д��������if����
		printf("\nthe file is not opened for write\n");			//�������д����Ϣ
		return 0;			//��������������
	}
	//add by liwen to check the filesize and alloc the BLOCK
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;			//�����ļ���һ�����Կ�ʼ��/д�ı��λ����off
	block = ((off+size)-inode->di_size)/BLOCKSIZ;				//���ڴ��������ڴ��ĸ���
	if(((off+size)-inode->di_size)%BLOCKSIZ)				//�������ĵ��ڴ�鲻������
		block++;			//�������ڴ��+1
	if(filsys.s_nfree < block){		//ϵͳ�еĿ��п�С�������
		printf("Not enough space to write so much bytes!\n");		//����ռ䲻���Ĵ���
		return 0;			//��������������
	}
	j = inode->di_size/BLOCKSIZ;		//jΪ�ļ�ռ���ڴ��ĸ���
	if(inode->di_size % BLOCKSIZ){		//���ռ�ĵ��ڴ�鲻������
		j ++;				//����+1
	}
	if(j+block>NADDR){			//�����ռ�ÿ���������Ҫ�����ȹ涨���ڴ��з���Ŀ�����
		printf("To write so much bytes will exceed the file limit!!\n");//�����������Ĵ���
		return 0;			//��������������
	}
	for(i=j;i<j+block;i++){			//������Ҫ���ڴ�鿪ʼִ�е����һ���ڴ��
		inode->di_addr[i]=balloc();	//���������ݿ�Ŀ�ż�Ϊ��ǰ�ļ���ʼ��λ��
	}
	inode->di_size += size;			//�����ļ�ȫ���Ĵ�С
	//end add
	temp_buf = buf;				//������������temp_buf

	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;			//�����ļ���һ�����Կ�ʼ��/д�ı��λ����off
	block_off = off % BLOCKSIZ;		//������ļ�����д�������ڴ���еĽ���λ��
	block = off/BLOCKSIZ;			//������ļ�����д����ռ�õĿ���

	if (block_off+size<BLOCKSIZ){		//���ʣ�಻��д�������д���ֿ��Դ���һ���ڴ����
		memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, buf, size);	//���ڴ���еĿ�д����Ҫ�������Ƶ���������
		return size;			//���ؿ�д����ռ���ڴ�Ĵ�С
	}
	memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,temp_buf,BLOCKSIZ-block_off);	//���������еĲ���һ���ڴ��Ĳ���д���ִ��������

	temp_buf += BLOCKSIZ-block_off;		//������������
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){			//��ÿ������д���ֵ��ڴ�����for������(size-(BLOCKSIZ-block_off))/BLOCKSIZΪ��д���ֿ�ʼ���ڴ���
		memcpy(disk+DATASTART+inode->di_addr[block+1+i]*BLOCKSIZ, temp_buf, BLOCKSIZ);	//���������е��ڴ�������Ҫ����д������λ��
		temp_buf += BLOCKSIZ;		//���󻺳������������Ա������һ���ڴ��
	}
	block_off = (size-(BLOCKSIZ-block_off)) % BLOCKSIZ;			//�ļ�����д�������ڴ���ռ�õĲ���һ�������Ĵ�С����block_off
	memcpy(disk+DATASTART+block*BLOCKSIZ, temp_buf, block_off);		//���������е����ݷ�����Ҫд�ĵط�
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;			//�����в��ֶ�������ļ���Ӧ��ϵͳ�洢�ļ����У����ҽ����в��ָ���Ϊ����д
	return size;				//������д����Ĵ�С
}


