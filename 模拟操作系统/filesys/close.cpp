
#include <stdio.h>
#include "filesys.h"

/******************************************************
������close
���ܣ��رմ��ļ�ʱ���޸��û��򿪱��ϵͳ�򿪱��еļ�¼��Ϣ
*******************************************************/
void close(unsigned int user_id, unsigned short cfd){
	struct inode *inode;									//����inode���ͽṹָ��inode
	inode = sys_ofile[user[user_id].u_ofile[cfd]].f_inode;//inodeָ��ָ����û��򿪵��ļ�
	if (inode)			//����Ӵ���
		iput(inode);	//����ָ��ָ����ڴ�i�ڵ�
	sys_ofile[user[user_id].u_ofile[cfd]].f_count -- ;	//ϵͳ�򿪱���ļ����ü���-1
	user[user_id].u_ofile[cfd] = SYSOPENFILE+1;			//�û��򿪱��м�¼ϵͳ�򿪱���ΪSYSOPENFILE+1
	return;
}
