#include <unistd.h> 
#include <stdio.h>
int main()
{
         int pid;                                                                                                                       
	int i;
	 while((pid=fork())==-1);
         if(pid==0)
	{	lockf(1,1,0);
                for(i=0;i<10000;i++){
		printf("b");
		}
		lockf(1,0,0);	
         }                                                                                                                    
         else if(pid>0)
         {
                 while((pid=fork())==-1);
                 if(pid==0)
                    {   
			lockf(1,1,0);
			for(i=0;i<10000;i++){
		        printf("c");
			}
			lockf(1,0,0);
		    }
                 else if(pid>0)
                   { 
			lockf(1,1,0);
			for(i=0;i<10000;i++){
			printf("a");
			}
			lockf(1,0,0);
		   }	
         }
	return 0;
}
