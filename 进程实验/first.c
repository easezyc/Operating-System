#include <unistd.h> 
#include <stdio.h>
int main()
{
         int pid;                                                                                                                       
         while((pid=fork())==-1);
         if(pid==0)
                printf("bbbbb");
                                                                                                                             
         else if(pid>0)
         {

                 while((pid=fork())==-1);
                 if(pid==0)
                        printf("ccccc");
                 else if(pid>0)
                        printf("aaaaa");
         }
	return 0;
}
