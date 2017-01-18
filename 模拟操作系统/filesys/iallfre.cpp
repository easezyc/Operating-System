#include <stdio.h>
#include <string.h>
#include "filesys.h"

static struct dinode block_buf[BLOCKSIZ/DINODESIZ];		//���i�ڵ����ʱ����
/*****************************************************
������ialloc
���ܣ��������i�ڵ㣬������Ӧ���ڴ�i�ڵ�ָ��
******************************************************/
struct inode * ialloc(){
	struct inode *temp_inode;//��ʱ����
	unsigned int cur_di; //����i�ڵ�
	int i,count, block_end_flag;//�������־,ֵΪ1ʱ��ʾ����i�ڵ���������

	//I������ʱ�ӵ�λ����λʹ�ã����ҷ����i�ڵ�Ҳ���ɵ͵���
	if (filsys.s_pinode == NICINOD){//�������ڿ���i�ڵ������С���ڿ���i�ڵ���
		i=0;
		block_end_flag = 1;//���������־��Ϊ1
		count = filsys.s_pinode = filsys.s_ninode>NICINOD ? 0 : (NICINOD - filsys.s_ninode);  //��countֵ=����i�ڵ���
		cur_di = filsys.s_rinode;//��cur_diΪ����i�ڵ�
		while (count < NICINOD){		//����i�ڵ�����û��װ���Ҵ����л��п���i�ڵ�
			if (block_end_flag){
				memcpy(block_buf, disk+DINODESTART + cur_di*DINODESIZ, BLOCKSIZ);//��i�ڵ�ȥ�ж�һ���̿鵽��ʱ����
				block_end_flag = 0;
				i = 0;
			}
			while (block_buf[IUPDATE].di_mode != DIEMPTY){	//��ʱ����Ϊ�գ����������i�ڵ�������  //i�ڵ�Ĵ�ȡȨ�޲����ڿ�Ȩ�ޣ�����
				cur_di ++;
				i++;
			}
			if (i == NICINOD){		//����i�ڵ���������
				block_end_flag = 1;
				continue;
			}
			filsys.s_inode[count++] = cur_di;
		}
		filsys.s_rinode = cur_di;	//����������i�ڵ�
	}
	/*�������i�ڵ�*/
	temp_inode = iget(filsys.s_inode[filsys.s_pinode]);
	memcpy(disk+DINODESTART+filsys.s_inode[filsys.s_pinode]*DINODESIZ,
	             &temp_inode->di_number, sizeof(struct dinode));
	filsys.s_pinode ++;
	filsys.s_ninode --; 
	filsys.s_fmod = SUPDATE; 
	return temp_inode;
} 

//�ͷų������е�i�ڵ�
void ifree(unsigned int dinodeid){
	filsys.s_ninode --;	//����i�ڵ�����һ
	if (filsys.s_pinode != 0){		//����i�ڵ�����δ��
		filsys.s_pinode--;
		filsys.s_inode[filsys.s_pinode] = dinodeid;//�����i�ڵ�����������ͷŵ�i�ڵ��
	}
	else{
		if (dinodeid < filsys.s_rinode){	
			//���ͷ�i�ڵ��С������i�ڵ�ţ�����ԭ����i�ڵ㣬���µ�����i�ڵ�Ϊ���ͷŵ�����i�ڵ�
			filsys.s_inode[NICINOD] = dinodeid;		//�����i�ڵ�����=���ͷŵ�i�ڵ��
			filsys.s_rinode = dinodeid;//������i�ڵ��=���ͷŵ�i�ڵ��
		}
	}
	return;
}















































