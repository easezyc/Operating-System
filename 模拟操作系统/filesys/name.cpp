
#include <stdio.h>
#include <string.h>
#include "filesys.h"
/* 
namei函数：参数：要查找的文件名。返回文件对应的内存目录项标号i；如果返回-1表示没有该文件。
功能：查找文件在当前目录下对应的内存目录项的号；
*/
int namei(char *name){
	int  i;

	for (i=0; i<DIRNUM; i++){
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))//名字匹配且目录项有效；
		{
			return i;	//返回在内存目录中的位置i（liwen change it ,userful in delete）
		}
	}
	return -1;  //没有找到
}
/* 
iname函数：功能：如果还有空目录项，为当前文件分配目录项；
           参数：将要分配的文件名。
		   返回：0没有空闲目录项；i分配的目录项号。
*/
unsigned short iname(char *name){
	int i, notfound = 1;

	for (i=0; ((i<DIRNUM) && (notfound)); i++){
		if (dir.direct[i].d_ino == 0)//该目录项未分配。
		{
			notfound = 0;//有空的目录项
			break;
		}
	}

	if (notfound){	//没有空目录项（notfound为1）时，执行
		printf("\nThe current directory is full!!!\n");
		return 0;
	}
	strcpy( dir.direct[i].d_name, name); 	//在notfound为0时（即存在空项），将要分配的文件名赋给该空项，表示此项被占用
	return i; 
}










































