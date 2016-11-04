#include <stdio.h>

#define getpch(type) (type*)malloc(sizeof(type))
//#define NULL 0

struct pcb   /* ������̿��ƿ�PCB */
{
    char name[10];
    char state;
    int  super;
    int  ntime;
    int  rtime;
    struct pcb * link;
}*ready=NULL,*p;

typedef struct pcb PCB;

void sort() /* �����Խ��̽������ȼ����к���*/
{
    PCB *first;
    first=ready;
    if (ready==NULL)
        ready=p;
    else
    {
        int mark=0;
        if(p->super>ready->super)
        {
            p->link=ready;
            ready=p;
        }
        else
        {
            while (first->link!=NULL)
            {
                if(p->super<=first->super&&p->super>first->link->super)
                {
                    p->link=first->link;
                    first->link=p;
                    mark=1;
                    break;
                }
                else
                {
                    first=first->link;
                }
            }
            //first=first->link;
            if(mark==0)first->link=p;
        }
    }
    return;
}

void input() /* �������̿��ƿ麯��*/
{
    int i,num;
    //clrscr(); /*����*/
    printf("\n ��������̸���?");
    scanf("%d",&num);
    for(i=0; i<num; i++)
    {
        printf("\n ���̺�No.%d:\n",i);
        p=getpch(PCB);
        printf("\n ���������:");
        scanf("%s",p->name);
        printf("\n �������������:");
        scanf("%d",&p->super);
        printf("\n �����������ʱ��:");
        scanf("%d",&p->ntime);
        printf("\n");
        p->rtime=0;
        p->state='w';
        p->link=NULL;
        sort(); /* ����sort����*/
    }
    return;
}

void disp(PCB * pr) /*����������ʾ����,������ʾ��ǰ����*/
{
    printf("\n qname \t state \t super \t ndtime \t runtime \n");
    printf(" |%s\t",pr->name);
    printf(" |%c\t",pr->state);
    printf(" |%d\t",pr->super);
    printf(" |%d\t",pr->ntime);
    printf("         |%d\t",pr->rtime);
    printf("\n");
    return;
}

void check() /* �������̲鿴���� */
{
    PCB* pr;
    printf("\n **** ��ǰ�������еĽ�����:%s",p->name); /*��ʾ��ǰ���н���*/
    disp(p);
    pr=ready;
    printf("\n ****��ǰ��������״̬Ϊ:\n"); /*��ʾ��������״̬*/
    while(pr!=NULL)
    {
        disp(pr);
        pr=pr->link;
    }
    return;
}

void destroy() /*�������̳�������(�������н���,��������)*/
{
    printf("\n ���� [%s] �����.\n",p->name);
    free(p);
    return;
}

void running() /* �������̾�������(��������ʱ�䵽,�þ���״̬*/
{
    (p->rtime)++;
    if(p->rtime==p->ntime)
        destroy(); /* ����destroy����*/
    else
    {
        p->state='w';
        p->super--;
        sort(); /*����sort����*/
    }
    return;
}

void main() /*������*/

{
    int h=0;
    char ch;
    input();
    ch=getchar();
    while(ready!=NULL)
    {
        h++;
        printf("\n The execute number:%d \n",h);
        p=ready;
        ready=p->link;
        p->link=NULL;
        p->state='R';
        check();
        running();
        printf("\n ����һ������......");
        ch=getchar();
    }
    printf("\n\n �����Ѿ����.\n");
    ch=getchar();
    return;
}
