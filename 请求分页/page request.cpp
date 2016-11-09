#include<iostream>
using namespace std;
const int N=64;
int pagesize;
struct pages
{
    int pNum; //ҳ��
    int bNum=-1; //������
    int dNum; //�ڴ����ϵ�λ��
    int write=0; //�޸ı�־
    int flag=0; //���ڱ�־
} page[N];
struct block
{
    int bNum;   //���
    int pNum;   //��Ӧ��ҳ��
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
    cout<<"ҳ��       �����������        ����λ           �޸ı�־"<<endl;
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
        cout<<"ҳ��Խ��"<<endl;
        return;
    }
    else
    {
        if(page[curp].flag!=0)
        {
            int paddress;
            paddress=page[curp].bNum*pagesize+pshift;
            cout<<"��ҳ���ڴ���,ҳ��:"<<curp<<"  ��Ӧ������:"<<page[curp].bNum<<"   ��Ӧ�����ַ:"<<paddress<<endl;
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
                cout<<"����"<<phyblock->pNum<<"ҳ����,���븨��λ��:"<<phy;
                page[phyblock->pNum].write=0;
            }
            else
            {
                cout<<"����"<<phyblock->pNum<<"ҳ����"<<endl;
            }
            int curblock=phyblock->bNum;
            phyblock=phyblock->next;
            insert(curblock,curp);
            page[curp].bNum=curblock;
            page[curp].flag=1;
            page[curp].write=write;
            int paddress;
            paddress=page[curp].bNum*pagesize+pshift;
            cout<<"����"<<curp<<"ҳ�����"<<curblock<<"��,��ǰָ�������ַ:"<<paddress<<endl;
        }
    }
}
int main()
{
    cout<<"������ҳ���С"<<endl;
    cin>>pagesize;
    int pn;
    cout<<"���������ҳ����"<<endl;
    cin>>pn;
    for(int i=0; i<pn; i++)
    {
        cout<<"�������"<<i<<"����λ��"<<endl;
        cin>>page[i].dNum;
        page[i].pNum=i;
    }
    int kn;
    do
    {
        cout<<"�����������ý��̵��������"<<endl;
        cin>>kn;
    }
    while(kn>=pn);
    for(int i=0; i<kn; i++)
    {
        cout<<"������������"<<endl;
        int t;
        cin>>t;
        insert(t,i);
        page[i].bNum=t;
        page[i].flag=1;
    }
    do
    {
        cout<<"����ָ���밴1���鿴��ǰ������������밴2���˳��밴-1"<<endl;
        int n;
        cin>>n;
        if(n==1)
        {
            int write;
            do
            {
                cout<<"������ָ���Ƿ��޸��ڴ�,�޸��밴1�����޸��밴0"<<endl;
                cin>>write;
            }
            while(write!=1&&write!=0);
            int address;
            cout<<"������ָ���ַ:"<<endl;
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
            cout<<"�������"<<endl;
        }
    }while(1);
    return 0;
}
