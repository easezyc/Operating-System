#include<iostream>
using namespace std;
const int N=64;
int pagesize;
struct pages
{
    int pNum; //页号
    int bNum=-1; //物理块号
    int dNum; //在磁盘上的位置
    int write=0; //修改标志
    int flag=0; //存在标志
} page[N];
struct block
{
    int bNum;   //块号
    int pNum;   //对应的页号
    block* next;
};
block* phyblock=NULL;
void insert(int bNum,int pNum)
{
    block* t=phyblock;
    if(phyblock==NULL)
    {
        phyblock=new block();
        phyblock->bNum=bNum;
        phyblock->pNum=pNum;
    }
    else
    {
        while(t!=NULL&&t->next!=NULL)
        {
            t=t->next;
        }
        block* tt=new block();
        tt->bNum=bNum;
        tt->pNum=pNum;
        t->next=tt;
    }
}
void print(int pn)
{
    cout<<"页号       分配的物理块号        存在位           修改标志"<<endl;
    for(int i=0; i<pn; i++)
    {
        cout<<page[i].pNum<<"           "<<page[i].bNum<<"              "<<page[i].flag<<"               "<<page[i].write<<endl;
    }
}
void request(int address,int write,int pn)
{
    int curp,pshift;
    curp=address/pagesize;
    pshift=address%pagesize;
    if(curp>pn)
    {
        cout<<"页号越界"<<endl;
        return;
    }
    else
    {
        if(page[curp].flag!=0)
        {
            int paddress;
            paddress=page[curp].bNum*pagesize+pshift;
            cout<<"该页在内存中,页号:"<<curp<<"  对应物理块号:"<<page[curp].bNum<<"   对应物理地址:"<<paddress<<endl;
            if(write==1)
            {
                page[curp].write=1;
            }
        }
        else
        {
            page[phyblock->pNum].flag=0;
            page[phyblock->pNum].bNum=-1;
            int phy=page[phyblock->pNum].dNum;
            if(page[phyblock->pNum].write==1)
            {
                cout<<"将第"<<phyblock->pNum<<"页调出,存入辅存位置:"<<phy;
                page[phyblock->pNum].write=0;
            }
            else
            {
                cout<<"将第"<<phyblock->pNum<<"页调出"<<endl;
            }
            int curblock=phyblock->bNum;
            phyblock=phyblock->next;
            insert(curblock,curp);
            page[curp].bNum=curblock;
            page[curp].flag=1;
            page[curp].write=write;
            int paddress;
            paddress=page[curp].bNum*pagesize+pshift;
            cout<<"将第"<<curp<<"页存入第"<<curblock<<"块,当前指令物理地址:"<<paddress<<endl;
        }
    }
}
int main()
{
    cout<<"请输入页面大小"<<endl;
    cin>>pagesize;
    int pn;
    cout<<"请输入程序页面数"<<endl;
    cin>>pn;
    for(int i=0; i<pn; i++)
    {
        cout<<"请输入第"<<i<<"辅存位置"<<endl;
        cin>>page[i].dNum;
        page[i].pNum=i;
    }
    int kn;
    do
    {
        cout<<"请输入分配给该进程的物理块数"<<endl;
        cin>>kn;
    }
    while(kn>=pn);
    for(int i=0; i<kn; i++)
    {
        cout<<"请输入物理块号"<<endl;
        int t;
        cin>>t;
        insert(t,i);
        page[i].bNum=t;
        page[i].flag=1;
    }
    do
    {
        cout<<"输入指令请按1，查看当前物理块分配情况请按2，退出请按-1"<<endl;
        int n;
        cin>>n;
        if(n==1)
        {
            int write;
            do
            {
                cout<<"请输入指令是否修改内存,修改请按1，不修改请按0"<<endl;
                cin>>write;
            }
            while(write!=1&&write!=0);
            int address;
            cout<<"请输入指令地址:"<<endl;
            cin>>address;
            request(address,write,pn);
        }
        else if(n==2)
        {
            print(pn);
        }
        else if(n==-1)
        {
            break;
        }
        else
        {
            cout<<"输入错误"<<endl;
        }
    }while(1);
    return 0;
}
