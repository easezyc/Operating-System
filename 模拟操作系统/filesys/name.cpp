
#include <stdio.h>
#include <string.h>
#include "filesys.h"
/* 
namei������������Ҫ���ҵ��ļ����������ļ���Ӧ���ڴ�Ŀ¼����i���������-1��ʾû�и��ļ���
���ܣ������ļ��ڵ�ǰĿ¼�¶�Ӧ���ڴ�Ŀ¼��ĺţ�
*/
int namei(char *name){
	int  i;

	for (i=0; i<DIRNUM; i++){
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))//����ƥ����Ŀ¼����Ч��
		{
			return i;	//�������ڴ�Ŀ¼�е�λ��i��liwen change it ,userful in delete��
		}
	}
	return -1;  //û���ҵ�
}
/* 
iname���������ܣ�������п�Ŀ¼�Ϊ��ǰ�ļ�����Ŀ¼�
           ��������Ҫ������ļ�����
		   ���أ�0û�п���Ŀ¼�i�����Ŀ¼��š�
*/
unsigned short iname(char *name){
	int i, notfound = 1;

	for (i=0; ((i<DIRNUM) && (notfound)); i++){
		if (dir.direct[i].d_ino == 0)//��Ŀ¼��δ���䡣
		{
			notfound = 0;//�пյ�Ŀ¼��
			break;
		}
	}

	if (notfound){	//û�п�Ŀ¼�notfoundΪ1��ʱ��ִ��
		printf("\nThe current directory is full!!!\n");
		return 0;
	}
	strcpy( dir.direct[i].d_name, name); 	//��notfoundΪ0ʱ�������ڿ������Ҫ������ļ��������ÿ����ʾ���ռ��
	return i; 
}










































