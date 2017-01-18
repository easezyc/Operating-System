#include <stdio.h>
#include "filesys.h"
/*********************************************************************
������delete
���ܣ�ɾ���ļ�
�������ļ�����filename
**********************************************************************/
void delete(char *filename){
	unsigned int dirid;
	struct inode *inode;

	dirid = namei(filename);//�����ļ��Ƿ����
	if (dirid == -1){
		printf("�ļ������ڣ�����!\n"); //�ļ������ڲ��˳�
		return;
	}
	inode = iget(dir.direct[dirid].d_ino);//����ָ�����ڴ�Ŀ¼���Ŷ�Ӧ�Ľڵ��ָ��
	//inode->i_count--;
	if(!(inode->di_mode &DIFILE)){//�������ļ�
		printf("�������ļ������飡\n");
		iput(inode);//���ս�㲢�˳�
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY; //ָ�����ڴ�Ŀ¼���Ŷ�Ӧ�Ľڵ��ָ������
	dir.size --;//Ŀ¼���С-1
	inode->di_number--;//�����ļ���-1
	iput(inode);//���ս�㲢�˳�
	return;
}











