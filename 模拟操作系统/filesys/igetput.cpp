#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h"

/* ��������	iget							*/
/* ���ã�	Ϊ����i�������Ӧ���ڴ�i���	*/
/* ����:	������Ĵ���i����				*/
/* ����ֵ��	ָ���Ӧ���ڴ�i����ָ��		*/
struct inode * iget(unsigned int dinodeid) //�ڴ�inode�Ĵ���
{
	int existed = 0, inodeid;//�ڴ�ڵ����ڵĹ�ϣ���к�
	long addr;
	struct inode *temp, *newinode;


	int i;


	inodeid = dinodeid % NHINO;//�����ڴ���Ӧ���ڵڼ�����ϣ�����NHINO�ǹ�ϣ������
	if (hinode[inodeid].i_forw == NULL)//���ù�ϣ����Ϊ�գ��ڴ���һ��δ��������hinode[]�������ǹ�ϣ���飬hinode[inodeid].i_forw����ָ��ϣ���к���inodeid��ָ��
		existed = 0;
	else//����Ϊ�գ��Ӹù�ϣ����ͷ��ʼ����
	{
		temp = hinode[inodeid].i_forw; 
		while (temp)
		{
			if (temp->i_ino == dinodeid)//���ҵ���temp->i_inoָtemp��ָ��Ĵ���i�Ľڵ�
			{
				existed = 1;
				temp->i_count ++;
				return temp;//���ظ��ڴ���ָ��
			}
			else
				temp = temp->i_forw;

		}
	}

	/* ��û���ҵ� */   
	/* 1. ����ô���i������ļ����е�λ�� */
	addr = DINODESTART + dinodeid * DINODESIZ;//����i�ڵ㿪ʼ��ַ+����i*�̿��С

	/* 2. ����һ���ڴ�i��� */
	newinode = (struct inode *)malloc(sizeof(struct inode));//��̬����һ���ڴ���

	/* 3. �ô���i����ʼ���ڴ�i��� */
	memcpy(&(newinode->di_number), disk+addr, DINODESIZ);

	/* 4. ���ڴ�i���������Ӧ�Ĺ�ϣ������*/
	newinode->i_forw = hinode[inodeid].i_forw;//���ڴ����ǰ��ָ��ָ��ǰ��ϣ����ָ�����ڵ�λ��
	hinode[inodeid].i_forw = newinode; //��ϣ����ָ���Ƶ��½���λ��
	newinode->i_back = newinode;//�½���β�����룬���Ժ���ָ��i_backָ����
	if (newinode->i_forw)//����ڴ������ڹ�ϣ���е�ǰ��Ҳ���н��
		newinode->i_forw->i_back = newinode;//ǰ����ĺ���ָ��ָ��ǰ���

	/*5. ��ʼ���ڴ�i�������������� */
	newinode->i_count = 1;//���ü���Ϊ1
	newinode->i_flag = 0;   /* ��ʾδ���� */
	newinode->i_ino = dinodeid;//�ڴ���i��Ӧ�Ĵ��̽����dinodeid

	return newinode;
}

/* ��������	iput							*/
/* ���ã�	�����ڴ�i���					*/
/* ����:	ָ������յ��ڴ�i���ָ��		*/
/* ����ֵ��	��								*/
void iput(struct inode *pinode)
{
	long addr;
	unsigned int block_num;
	int i;
	if (pinode->i_count > 1)//�����ü���>1
	{
		pinode->i_count --;
		return;
	}
	else
	{
		if (pinode->di_number != 0)//�����������Ϊ0
		{
			/* ���ڴ�i��������д�ش���i��� */
			addr = DINODESTART + pinode->i_ino *DINODESIZ;//�����ַ=��ʼ��ַ+���i*���С
			memcpy(disk+addr, &pinode->di_number, DINODESIZ);//�����ļ������ݿ��뵽������
		}
		else//�����ļ�����0��ɾ���ڵ���ļ�
		{
			/* ɾ������i�����ļ���Ӧ������� */
			block_num = pinode->di_size/BLOCKSIZ;
			for (i = 0; i <= block_num; i++)
			{
				bfree(pinode->di_addr[i]);//�ͷ������
			}
			if (pinode->di_size%BLOCKSIZ != 0){
				//bfree(pinode->di_addr[i]);
			}
			ifree(pinode->i_ino);//�ͷŴ��̽��i
		}

		/* �ͷ��ڴ�i��� */
		{
			int inodeid;
			inodeid = (pinode->i_ino)  % NHINO ;//�ҵ����ڵĹ�ϣ����

			/* �Ӹù�ϣ������ɾ�� */
			if (hinode[inodeid].i_forw == pinode)//�����ǰ����ڹ�ϣ���е���β��
			{
				hinode[inodeid].i_forw = pinode->i_forw;//�ù�ϣ���е�ǰָ��ָ��pinode��ǰһ�����
				if (pinode->i_forw)//���ǰһ��������
					pinode->i_forw->i_back = pinode->i_forw;//ǰһ�����ĺ���ָ��ָ�����Լ�
			}
			else//�����ǰ����ڹ�ϣ�����м�
			{
				pinode->i_back->i_forw = pinode->i_forw;//��ǰ������ָ����ָ��Ľ��ָ��ǰ����ǰһ�����
				if (pinode->i_forw)//���ǰһ��������
					pinode->i_forw->i_back = pinode->i_back;//���ĺ���ָ��ָ��ǰ���ĺ�һ�����
			}
		}
		free(pinode);
	}

	return;
}













