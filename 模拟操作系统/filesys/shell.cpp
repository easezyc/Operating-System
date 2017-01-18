//#include 
#include "filesys.h"
#define CLEN 10
#define CNUM 20
//enum ctype
char commands[CNUM][CLEN]={
"exit",
"ls",
"mkdir",
"cd",
"touch",
"rm",
"write",
"read",
"su",
"passwd",
"usermod",
"adduser",
"who",
"cat",
"help"
};
int getcid(char *command){
	int i;
	if(command == NULL)
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int userid,char *str){
	char seps[] =" \t\n\0";
	char *token,*tstr,*buf;
	char* oldpwd = (char *)malloc(sizeof(char));
	char* pwd2 = (char *)malloc(sizeof(char));
	char* pwd1 = (char *)malloc(sizeof(char));
	char* name = (char *)malloc(sizeof(char));
	unsigned short mode,fd;
	int cid,size,num,mark;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir�������ȷ��ʽΪmkdir dirname����������!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd�������ȷ��ʽΪcd dirname����������!\n");
			break;
		}
		chdir(token);
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("touch �������ȷ��ʽΪtouch filename [mode]����������!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(userid,tstr,mode);
		if(fd == -1){
			printf("�����ļ�ʧ�ܣ�\n");
			break;
		}
		close(userid,fd);
		break;
	case 5:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("rm �������ȷ��ʽΪrm filename����������!\n");
			break;
		}
		while (token != NULL)
		{
			delete(token);
			token = strtok(NULL, seps);
		}
		break;
	case 6:
		mode = WRITE;
		token = strtok(NULL, seps);
		tstr = token;
		token = strtok(NULL, seps);
		if (token == NULL){
			printf("write �������ȷ��ʽΪwrite filename bytes����������!\n");
			break;
		}
		if (token[0] == '-'){
			if (token[1] == 'a')
				mode = FAPPEND;
		}
		else{
			sscanf(token, "%d", &size); //size ��bytes
		}
		fd = open(userid, tstr, char(mode));
		if (fd == 65535) //��Ϊfs��unsigned short
			break;       //���fdΪ-1˵�� ������ ���Ǹ��ļ� û��Ȩ��  
		if (size < 0){
			printf("file size have to be > 0!!\n");
			close(userid, fd);
			break;
		}
		token = strtok(NULL, seps);//token�����д����ַ��� 
		if (token != NULL) {
			if (strlen(token) > size) {  //���������ַ������ȴ���size 
				size = strlen(token);
			}
			buf = (char*)malloc(size);
			strcpy(buf, token);
			size = write(fd, buf, size);
			printf("%d bytes have been writed in file %s.\n", size, tstr);
			close(userid, fd);
			break;
		}
		else{
			buf = (char*)malloc(size);
			buf[0] = '\0';
			size = write(fd, buf, size);
			close(userid, fd);
			printf("%d bytes have been writed in file %s.\n", size, tstr);
			break;
		}

	case 7:
		token = strtok(NULL, seps);
		tstr = token;
		token = strtok(NULL, seps);
		if (token == NULL){
			printf("read �������ȷ��ʽΪread filename bytes����������!\n");
			break;
		}
		sscanf(token, "%d", &size);
		fd = open(userid, tstr, READ);
		if (fd == 65535) //��Ϊfs��unsigned short
			break;
		if (size < 0) {
			printf("file size have to be > 0!!\n");
			close(userid, fd);
			break;
		}
		buf = (char*)malloc(size + 1);
		size = read(fd, buf, size);
		printf("%d bytes have been read in buf from file %s.\n", size, tstr);
		buf[size] = '\0';
		printf("%s\n", buf);
		free(buf);
		close(userid, fd);
		break;
	case 8:
		token = strtok(NULL, seps);
		if (token == NULL){
			num = 2116;
			printf("����:");
			scanf("%s", pwd2);
			mark = login(num, pwd2);
			if (mark == -1);
			else user_id = mark;
			break;
		}
		tstr = token;
		num = atoi(tstr);
		printf("����:");
		scanf("%s", pwd2);
		if (num == 0)
		{
			mark = login2(tstr, pwd2);
			if (mark == -1);
			else user_id = mark;
			break;
		}
		mark = login(num, pwd2);
		if (mark == -1);
		else user_id = mark;
		break;
	case 9:
		printf("Old password:");
		scanf("%s", oldpwd);
		if (strcmp(oldpwd, user[userid].password) != 0){
			printf("�����������\n");
			break;
		}
		printf("New password:");
		scanf("%s", pwd2);
		printf("Retype new password:");
		scanf("%s", pwd1);
		if (strcmp(pwd2, pwd1) == 0)modpwd(user[userid].u_uid,pwd2);
		else printf("������������벻ͬ\n");
		break;
	case 10:
		token = strtok(NULL, seps);
		if (token == NULL){		//��
			printf("����ȷ�����ʽ��usermod -g group:\n");
			break;
		}
		if (token[0] == '-'){
			if (token[1] != 'g')
			{
				printf("��Ҫ����-g�����޸��û���\n");
				break;
			}
		}
		else{
			printf("��Ҫ����-g�����޸��û���\n");
			break;
		}
		token = strtok(NULL, seps);
		if (token == NULL){
			printf("usermod �������ȷ��ʽΪusermod -g groupnum����������!\n");
			break;
		}
		num = atoi(token);
		modgroup(user[userid].u_uid,num);
		user[userid].u_gid = num;
		break;
	case 11:
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			printf("adduser �������ȷ��ʽΪadduser username (-u uid)����������!\n");
			break;
		}
		strcpy(name, token);
		token = strtok(NULL, seps);
		if (token != NULL){
			if (token[0] == '-'){
				if (token[1] == 'u')
				{
					token = strtok(NULL, seps);
					if (token == NULL){
						printf("��δָ��uid\n");
						break;
					}
					num = atoi(token);
					if (num == 0)
					{
						printf("�����uid����������\n");
						break;
					}
					if (adduser2(name, num) == 0)printf("�����û�ʧ�ܣ��û�������uid�Ѵ���\n");
					break;
				}
				else {
					printf("��������ֻ��Ϊ-u\n");
					break;
				}
			}
		}
		if(adduser1(name)==0)printf("�����û�ʧ�ܣ��û�������uid�Ѵ���\n");
		break;
	case 12:
		printf("username     userid      userpwd      usergroup\n");
		printf("%s           %d            %s              %d\n", user[userid].u_name, user[userid].u_uid, user[userid].password, user[userid].u_gid);
		break;
	case 13:
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			printf("cat�������ȷ��ʽΪcat filename����������!\n");
			break;
		}
		if (strcmp(token, "/etc/passwd") == 0)
		{
			printf("username     userid      userpwd      usergroup\n");
			int ii = 0;
			for (; ii < usernum; ii++)
			{
				printf("%s           %d          %s              %d\n", pwd[ii].p_name, pwd[ii].p_uid, pwd[ii].password, pwd[ii].p_gid);
			}
		}
		break;
	case 14:
		printf("exit �˳�\n");
		printf("ls �鿴��ǰĿ¼\n");
		printf("mkdir dirname �½�Ŀ¼\n");
		printf("cd dirname ��Ŀ¼\n");
		printf("touch filename ��mode) �½��ļ�,modeΪȨ��\n");
		printf("rm filenames ɾ��һ�����߶���ļ�\n");
		printf("write filename (bytes string) д���ļ�\n");
		printf("read filename bytes ���ļ�\n");
		printf("su (username)�л��û�\n");
		printf("passwd �޸ĵ�ǰ�û�����\n");
		printf("usermod -g group �޸��û���\n");
		printf("adduser username ��-u uid���½��û�������ָ��uid��Ĭ������Ϊ123\n");
		printf("who �鿴��ǰ�û���Ϣ\n");
		printf("cat /etc/passwd �鿴�����û���Ϣ\n");
		printf("help �鿴��������\n");
		break;
	case 0:
		return 0;
	default:
		printf("����:û������%s��\n",token);
		break;
	}
	return 1;
}
