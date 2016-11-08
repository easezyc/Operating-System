#include<iostream>
using namespace std;
const long size=50;
struct list
{
    string name;
    long curaddress;
    long length;
    list* next=NULL;
};
list* frelist;
list* processlist;
void insert(string name,long address,long length)  //�����µĽ���
{
    list* t=processlist;
    if(processlist==NULL)
    {
        processlist=new list();
        processlist->curaddress=address;
        processlist->length=length;
        processlist->name=name;
    }
    else if(processlist->curaddress>address)
    {
        list* t=new list();
        t->curaddress=address;
        t->length=length;
        t->name=name;
        t->next=processlist;
        processlist=t;
    }
    else
    {
        list* temp=processlist;
        while(temp!=NULL)
        {
            if(temp->curaddress<address)
            {
                if(temp->next==NULL)
                {
                    list* t=new list();
                    t->curaddress=address;
                    t->length=length;
                    t->name=name;
                    temp->next=t;
                    break;
                }
                else if(address<temp->next->curaddress)
                {
                    list* t=new list();
                    t->curaddress=address;
                    t->length=length;
                    t->name=name;
                    t->next=temp->next;
                    temp->next=t;
                    break;
                }
                else
                {
                    temp=temp->next;
                }
            }
        }
    }
}
void allocation(string name,long length)   //���̷���
{
    list* t=frelist;
    list* pre=frelist;
    int mark=0;
    long address=-1;
    if(frelist!=NULL)
    {
        if(frelist->length>=length)
        {
            if(frelist->length>length+size)
            {
                frelist->length=frelist->length-length;
                address=frelist->curaddress;
                frelist->curaddress=frelist->curaddress+length;
                mark=1;
            }
            else
            {
                address=frelist->curaddress;
                length=frelist->length;
                frelist=frelist->next;
                mark=1;
            }
        }
    }
    if(mark==0)
    {
        t=t->next;
        while(t!=NULL)
        {
            if(t->length>=length)
            {
                if(t->length>length+size)
                {
                    t->length=t->length-length;
                    address=t->curaddress;
                    t->curaddress=t->curaddress+length;
                    mark=1;
                    break;
                }
                else
                {
                    address=t->curaddress;
                    length=t->length;
                    pre->next=t->next;
                    mark=1;
                    break;
                }
            }
            t=t->next;
            pre=pre->next;
        }
    }
    if(mark==1)
    {
        insert(name,address,length);

    }
    else
    {
        cout<<"û�п��������Է���"<<endl;
    }
}
void recycling(string name)   //���ս��̷���
{
    list* temp=processlist;
    list* pre=processlist;
    long address;
    long length;
    int mark=0;
    if(processlist->name==name)
    {
        address=processlist->curaddress;
        length=processlist->length;
        mark=1;
        processlist=processlist->next;
    }
    else
    {
        temp=temp->next;
        while(temp!=NULL)
        {
            if(temp->name==name)
            {
                address=temp->curaddress;
                length=temp->length;
                mark=1;
                pre->next=temp->next;
                break;
            }
            else
            {
                temp=temp->next;
                pre=pre->next;
            }
        }
    }
    if(mark==1)
    {
        if(frelist==NULL)
        {
            frelist=new list();
            frelist->curaddress=address;
            frelist->length=length;
        }
        else
        {
            if(address<frelist->curaddress)
            {
                if(address+length==frelist->curaddress)
                {
                    frelist->length=frelist->length+length;
                    frelist->curaddress=address;
                }
                else
                {
                    list* t=new list();
                    t->length=length;
                    t->curaddress=address;
                    t->next=frelist;
                    frelist=t;
                }
            }
            else
            {
                list* t=frelist;
                while(t!=NULL)
                {
                    if(t->curaddress<address)
                    {
                        if(t->next==NULL)
                        {
                            if(t->curaddress+t->length==address)
                            {
                                t->length=t->length+length;
                            }
                            else
                            {
                                list* tt=new list();
                                tt->length=length;
                                tt->curaddress=address;
                                t->next=tt;
                                break;
                            }
                        }
                        else
                        {
                            if(address<t->next->curaddress)
                            {
                                t->length==t->length+length;
                                if(t->curaddress+t->length==address)
                                {
                                    if(t->curaddress+t->length==t->next->curaddress)
                                    {
                                        list* tt=t->next;
                                        t->length+=tt->length;
                                        t->next=tt->next;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    t=t->next;
                }
            }
        }
    }
    else if(mark==0)
    {
        cout<<"û����ؽ���"<<endl;
    }
}
void trace()   //�������ж��кͽ��̶�����ʾ
{
    cout<<"���з�����:"<<endl;
    cout<<"��ַ         ����"<<endl;
    list* t=frelist;
    while(t!=NULL)
    {
        cout<<t->curaddress<<"         "<<t->length<<endl;
        t=t->next;
    }
    cout<<"���̷�����:"<<endl;
    cout<<"��������    ��ַ         ����"<<endl;
    t=processlist;
    while(t!=NULL)
    {
        cout<<t->name<<"          "<<t->curaddress<<"          "<<t->length<<endl;
        t=t->next;
    }
}
int main()
{
    frelist=new list();
    cout<<"�������ڴ���ʼ��ַ�Լ��ܳ���"<<endl;
    long address;
    long length;
    string name;
    cin>>address>>length;
    frelist->curaddress=address;
    frelist->length=length;
    int n;
    cout<<"��������ռ�����1�����տռ�����2����ʾ�����б�ͽ����б�����3���˳�����-1"<<endl;
    cin>>n;
    while(1)
    {
        if(n==1)
        {
            cout<<"����������ֺͳ���"<<endl;
            cin>>name>>length;
            allocation(name,length);
        }
        else if(n==2)
        {
            cout<<"�����������"<<endl;
            cin>>name;
            recycling(name);
        }
        else if(n==3)
        {
            trace();
        }
        else if(n==-1)
        {
            break;
        }
        else
        {
            cout<<"�������"<<endl;
        }
        cout<<"��������ռ�����1�����տռ�����2����ʾ�����б�ͽ����б�����3���˳�����-1"<<endl;
        cin>>n;
    }
}
