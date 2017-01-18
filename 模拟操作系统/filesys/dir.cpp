#include <stdio.h>
#include <string.h>
#include "filesys.h"
/*********************************************************************
������dir
���ܣ���Ŀ¼

**********************************************************************/
void _dir(){
	unsigned int di_mode;
	int i,j,k;          //xiao
	struct inode *temp_inode;

	printf("\n CURRENT DIRECTORY :%s\n",dir.direct[0].d_name);
	printf("��ǰ����%d���ļ�/Ŀ¼\n",dir.size);
	for (i=0; i<DIRNUM; i++){
		if (dir.direct[i].d_ino != DIEMPTY){//��ӡĿ¼ֱ���ڵ��־Ϊ��
			printf("%-14s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			di_mode = temp_inode->di_mode & 00777;	//��λ�ļ�����
			for (j=0; j<9; j++){//�ӵ�λ��ʼ��Ϊ1�����X��Ϊ0���-��Ĭ��Ȩ��111111111-->xxxxxxxx
				if (di_mode%2){
					printf("x");
				}else{
					printf("-");
				}
				di_mode = di_mode/2;
			}
			printf("\ti_ino->%d\t",temp_inode->i_ino);//����ڵ��־��
			if (temp_inode->di_mode & DIFILE){//������ļ���������ļ���С�Ϳ�����
				printf(" %ld ", temp_inode->di_size);
				printf("block chain:");
				j=(temp_inode->di_size%BLOCKSIZ)?1:0;//�ļ���С�Ƿ��ǿ��ֽڵ������������ǵĻ�����
				for (k=0; k < temp_inode->di_size/BLOCKSIZ+j; k++)
					printf("%4d", temp_inode->di_addr[k]);//������
				printf("\n");
			}else{
				printf("<dir>\n");//�����ļ��Ļ������<Ŀ¼>����
			}//else
			iput(temp_inode);//����ָ��
		}// if (dir.direct[i].d_ino != DIEMPTY)
	}//for
	return;
}
/*********************************************************************
������mkdir
���ܣ��½�Ŀ¼
������Ŀ¼����
���أ���
**********************************************************************/
void mkdir(char *dirname){
	int dirid, dirpos;
	struct inode *inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	unsigned int block;

	dirid= namei(dirname);//���ض�Ӧ���ڴ�Ŀ¼����
	if (dirid != -1){//����ͬ��Ŀ¼/�ļ�
		inode = iget(dirid);
		if (inode->di_mode & DIDIR) //����ͬ��Ŀ¼
			printf("Ŀ¼%s������ͻ��\n", dirname); //xiao
		else
			printf("%s��һ���ļ���\n", dirname);//����ͬ���ļ�
		iput(inode);//����ָ�벢�˳�
		return;
	}
	dirpos = iname(dirname);					//ȡ����addr�еĿ�����λ��,����Ŀ¼��д��������
	inode = ialloc();							//����i�ڵ�
	dir.direct[dirpos].d_ino = inode->i_ino;	//���ø�Ŀ¼�Ĵ���i�ڵ��
	dir.size++;									//Ŀ¼��++

	strcpy(buf[0].d_name,"..");					//��Ŀ¼����һ��Ŀ¼ ��ǰĿ¼
	buf[0].d_ino = cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;				//��Ŀ¼�ı�Ŀ¼ ��Ŀ¼

	block = balloc();
	memcpy(disk+DATASTART+block*BLOCKSIZ, buf, BLOCKSIZ); //����������ݿ鲢���ؿ��
    //�ڴ濽�����ӵ�ǰ��ջ������ָ��λ��һ�����С������
	inode->di_size = 2*(DIRSIZ+4);//���ô�С
	inode->di_number = 1; //�����ļ�������
	inode->di_mode = user[user_id].u_default_mode|DIDIR;//����Ȩ��
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;

	iput(inode);
	return;
}
/*********************************************************************
������chdir
���ܣ�cd����鿴Ŀ¼
������Ŀ¼����
���أ���
**********************************************************************/

void chdir(char *dirname){
	unsigned int dirid;
	struct inode *inode;
	unsigned short block;
	int i,j,low=0, high=0;

	dirid = namei(dirname);//����Ŀ¼��
	if (dirid == -1){
		printf("������Ŀ¼%s��\n", dirname);
		return; //������Ŀ¼���˳�
	}
	inode =iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode&DIDIR)){
		printf("%s����һ��Ŀ¼��\n",dirname);  //���ļ�����Ŀ¼���˳�
		return;
	}
	for (i=0; i<dir.size; i++){ //pack��ǰĿ¼
		if(dir.direct[i].d_ino == 0){
			for(j=DIRNUM-1;j>=0&&dir.direct[j].d_ino == 0;j--);//dirnumĿ¼���������
			memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ+4);  //xiao
			dir.direct[j].d_ino = 0;
		}
	}
	j = cur_path_inode->di_size%BLOCKSIZ?1:0; //�̿���
	//д�ص�ǰĿ¼
	for (i=0; i<cur_path_inode->di_size/BLOCKSIZ+j; i++){
		bfree(cur_path_inode->di_addr[i]);//���ͷŵ��̿��ѹ����п��ջ��
	}
	for (i=0; i<dir.size; i+=BLOCKSIZ/(DIRSIZ+4)){
		block = balloc(); //��������鲢���ؿ��
		cur_path_inode->di_addr[i] = block;
		memcpy(disk+DATASTART+block*BLOCKSIZ, &dir.direct[i], BLOCKSIZ);//���ʽڵ㣬��������Ӧ��ַ
	}
	cur_path_inode->di_size = dir.size*(DIRSIZ+4);
	iput(cur_path_inode);//�ͷŽڵ�
	cur_path_inode = inode;//���ô��������ڵ�Ϊ��ǰ·���ڵ�

	j=0; //�Ӵ����ж����ı�Ŀ¼
	for (i=0; i<inode->di_size/BLOCKSIZ+1; i++){
		memcpy(&dir.direct[j],disk+DATASTART+inode->di_addr[i]*BLOCKSIZ, BLOCKSIZ); //�������ڵ��д�뵱ǰĿ¼��
		j+=BLOCKSIZ/(DIRSIZ+4);
	}
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);
	for (i=dir.size; i<DIRNUM; i++){
		dir.direct[i].d_ino = 0;//������ǰĿ¼��С��Ŀ¼��������ڵ���ȫ������
	}
	if (strcmp(dirname, "..") == 0){//�����ǰ·��
		struct list* t= namelist;
		struct list* tt = namelist;
		tt = tt->next;
		if (tt != NULL)
		{
			while (tt->next != NULL)
			{
				tt = tt->next;
				t = t->next;
			}
			t->next = NULL;
		}
	}
	else if (strcmp(dirname, ".") == 0){

	}
	else{
		struct list* t = namelist;
		while (t->next != NULL)
		{
			t = t->next;
		}
		struct list* tt = (struct list*)malloc(sizeof(struct list));
		tt->next = NULL;
		strcpy(tt->name, dirname);
		t->next = tt;
	}
	//end by xiao

	return;
}
/*********************************************************************
������trace
���ܣ����浱ǰ·��
�������û�ID������
���أ���
**********************************************************************/
void trace(int uid,char* name)
{
	struct list* t = namelist;//�½��ṹ��
	t = t->next;
	int mark = 0;
	while (t != NULL)
	{
		if (strcmp(t->name, "home") == 0 && t->next != NULL&&strcmp(t->next->name, name) == 0){
			mark = 1;//��ǰ�ڸ�Ŀ¼��
			break;
		}
		t = t->next;
	}
	t = namelist;
	t = t->next;
	if (uid == 2116)//�����û�   #
	{
		printf("/");
		while (t != NULL)
		{
			printf("%s", t->name);
			if (t->next != NULL)printf("/");
			t = t->next;//�����ǰ·��
		}
		printf("#");
	}
	else//��ͨ�û������Ե�Ŀ¼�� ~$
	{
		if (mark == 1)
		{
			printf("~");
			int mark1 = 0;
			t = t->next;
			t = t->next;
			while (t != NULL)
			{
				if (mark1 == 0)
				{
					printf("/");
					mark1 = 1;
				}
				printf("%s", t->name);
				if (t->next != NULL)printf("/");
				t = t->next;
			}
			printf(" $");
		}
		else{
			printf("/");
			while (t != NULL)
			{
				printf("%s", t->name);
				if (t->next != NULL)printf("/");
				t = t->next;
			}
			printf(" $");
		}
	}
}









































