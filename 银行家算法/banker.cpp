#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
int Available[50];           //��ʹ����Դ����
int Max[50][50];             //����������
int Allocation[50][50]= {0};     //�������
int Need[50][50]= {0};           //�������
int Work[50];                //��������
int Finish[50];              //״̬��־
int Request[50][50];         //����������Դ����
int list[50];
int rnum,pnum;
void putin()
{
    cout<<"��������Դ������:";
    cin>>rnum;
    cout<<"��������̵�����:";
    cin>>pnum;
    cout<<"����������"<<rnum<<"����Դ������"<<endl;
    for(int i=1; i<=rnum; i++)
    {
        cout<<"��"<<i<<"����Դ:";
        cin>>Available[i];
    }
    cout<<"����������"<<pnum<<"�����̵����������Դ����"<<endl;
    for(int i=1; i<=pnum; i++)
    {
        for(int j=1; j<=rnum; j++)
        {
            cout<<"��"<<i<<"�����������"<<j<<"����Դ����:";
            cin>>Max[i][j];
            Need[i][j]=Max[i][j];
        }
    }
}
void test(int tp)
{
    for(int i=1; i<=rnum; i++)
    {
        Work[i]=Available[i];
        Work[i]=Work[i]-Request[tp][i];
        Allocation[tp][i]+=Request[tp][i];
        Need[tp][i]-=Request[tp][i];
    }
    for(int i=1; i<=pnum; i++)
    {
        Finish[i]=0;
    }
    int num=0;
    for(int i=1; i<=pnum; i++)
    {
        for(int j=1; j<=pnum; j++)
        {
            if(Finish[j]==0)
            {
                int count=0;
                for(int g=1; g<=rnum; g++)
                {
                    if(Need[j][g]<=Work[g])count++;
                }
                if(count==rnum)
                {
                    num++;
                    list[num]=j;
                    Finish[j]=1;
                    for(int g=1;g<=rnum;g++)
                    {
                        Work[g]+=Allocation[j][g];
                    }
                }
            }
        }
    }
    if(num==pnum)
    {
        cout<<"��ȫ����Ϊ:";
        for(int i=1; i<=pnum; i++)
        {
            cout<<list[i]<<"   ";
        }
        cout<<endl;
        for(int i=1;i<=rnum;i++)
        {
            Available[i]-=Request[tp][i];
        }
    }
    else
    {
        cout<<"�ް�ȫ����"<<endl;
        for(int i=1;i<=rnum;i++)
        {
            Allocation[tp][i]-=Request[tp][i];
            Need[tp][i]+=Request[tp][i];
        }
    }
}

void request()
{
    cout<<"�����뷢������Ľ��̺�:";
    int tp;
    cin>>tp;
    if(tp>pnum)
    {
        cout<<"û���������"<<endl;
        return;
    }
    cout<<"������ý��̶Ը�����Դ���������"<<endl;
    for(int i=1; i<=rnum; i++)
    {
        cout<<"��"<<i<<"����Դ����:";
        int mark = 0;
        int t;
        while(mark==0)
        {
            cin>>t;
            if(t>Need[tp][i])
            {
                cout<<"�����������ܴ���������"<<endl;
                mark=0;
            }
            else if(t>Available[i])
            {
                cout<<"�����������ܴ��ڿɻ����"<<endl;
                mark=0;
            }
            else mark=1;
        }
        Request[tp][i]=t;
    }
    test(tp);
}
void putout()
{
    cout<<"���̺�              �������               ������                 �ѷ���"<<endl;
    for(int i=1;i<=pnum;i++)
    {
        cout<<i<<"               ";
        for(int j=1;j<=rnum;j++)
        {
            cout<<Max[i][j]<<"  ";
        }
        cout<<"                  ";
        for(int j=1;j<=rnum;j++)
        {
            cout<<Need[i][j]<<"  ";
        }
        cout<<"                  ";
        for(int j=1;j<=rnum;j++)
        {
            cout<<Allocation[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"��ϵͳ��Դʣ����:";
    for(int j=1;j<=rnum;j++)
    {
        cout<<Available[j]<<"  ";
    }
    cout<<endl;
}
int main()
{
    putin();
    char key;
    cout<<"�˳�����e,�鿴��ǰ״̬����a,������������r"<<endl;
    cin>>key;
    while(key!='e')
    {
        if(key=='a')
        {
            putout();
        }
        else if(key='r')
        {
            request();
        }
        cout<<"�˳�����e,�鿴��ǰ״̬����a,������������r"<<endl;
        cin>>key;
    }
    return 0;
}
