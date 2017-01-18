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
			printf("mkdir命令的正确格式为mkdir dirname，请检查命令!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd命令的正确格式为cd dirname，请检查命令!\n");
			break;
		}
		chdir(token);
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("touch 命令的正确格式为touch filename [mode]，请检查命令!\n");
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
			printf("创建文件失败！\n");
			break;
		}
		close(userid,fd);
		break;
	case 5:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("rm 命令的正确格式为rm filename，请检查命令!\n");
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
			printf("write 命令的正确格式为write filename bytes，请检查命令!\n");
			break;
		}
		if (token[0] == '-'){
			if (token[1] == 'a')
				mode = FAPPEND;
		}
		else{
			sscanf(token, "%d", &size); //size 是bytes
		}
		fd = open(userid, tstr, char(mode));
		if (fd == 65535) //因为fs是unsigned short
			break;       //如果fd为-1说明 不存在 不是个文件 没有权限  
		if (size < 0){
			printf("file size have to be > 0!!\n");
			close(userid, fd);
			break;
		}
		token = strtok(NULL, seps);//token存的是写入的字符串 
		if (token != NULL) {
			if (strlen(token) > size) {  //如果输入的字符创长度大于size 
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
			printf("read 命令的正确格式为read filename bytes，请检查命令!\n");
			break;
		}
		sscanf(token, "%d", &size);
		fd = open(userid, tstr, READ);
		if (fd == 65535) //因为fs是unsigned short
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
			printf("密码:");
			scanf("%s", pwd2);
			mark = login(num, pwd2);
			if (mark == -1);
			else user_id = mark;
			break;
		}
		tstr = token;
		num = atoi(tstr);
		printf("密码:");
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
			printf("密码输入错误\n");
			break;
		}
		printf("New password:");
		scanf("%s", pwd2);
		printf("Retype new password:");
		scanf("%s", pwd1);
		if (strcmp(pwd2, pwd1) == 0)modpwd(user[userid].u_uid,pwd2);
		else printf("两次输入的密码不同\n");
		break;
	case 10:
		token = strtok(NULL, seps);
		if (token == NULL){		//朱
			printf("请正确输出格式：usermod -g group:\n");
			break;
		}
		if (token[0] == '-'){
			if (token[1] != 'g')
			{
				printf("需要输入-g才能修改用户组\n");
				break;
			}
		}
		else{
			printf("需要输入-g才能修改用户组\n");
			break;
		}
		token = strtok(NULL, seps);
		if (token == NULL){
			printf("usermod 命令的正确格式为usermod -g groupnum，请检查命令!\n");
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
			printf("adduser 命令的正确格式为adduser username (-u uid)，请检查命令!\n");
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
						printf("还未指定uid\n");
						break;
					}
					num = atoi(token);
					if (num == 0)
					{
						printf("输入的uid必须是数字\n");
						break;
					}
					if (adduser2(name, num) == 0)printf("创建用户失败，用户名或者uid已存在\n");
					break;
				}
				else {
					printf("参数错误，只能为-u\n");
					break;
				}
			}
		}
		if(adduser1(name)==0)printf("创建用户失败，用户名或者uid已存在\n");
		break;
	case 12:
		printf("username     userid      userpwd      usergroup\n");
		printf("%s           %d            %s              %d\n", user[userid].u_name, user[userid].u_uid, user[userid].password, user[userid].u_gid);
		break;
	case 13:
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			printf("cat命令的正确格式为cat filename，请检查命令!\n");
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
		printf("exit 退出\n");
		printf("ls 查看当前目录\n");
		printf("mkdir dirname 新建目录\n");
		printf("cd dirname 打开目录\n");
		printf("touch filename （mode) 新建文件,mode为权限\n");
		printf("rm filenames 删除一个或者多个文件\n");
		printf("write filename (bytes string) 写入文件\n");
		printf("read filename bytes 打开文件\n");
		printf("su (username)切换用户\n");
		printf("passwd 修改当前用户密码\n");
		printf("usermod -g group 修改用户组\n");
		printf("adduser username （-u uid）新建用户，可以指定uid，默认密码为123\n");
		printf("who 查看当前用户信息\n");
		printf("cat /etc/passwd 查看所有用户信息\n");
		printf("help 查看所有命令\n");
		break;
	case 0:
		return 0;
	default:
		printf("错误:没有命令%s！\n",token);
		break;
	}
	return 1;
}
