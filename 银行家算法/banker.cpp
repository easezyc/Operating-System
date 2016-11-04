#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
int Available[50];           //可使用资源向量
int Max[50][50];             //最大需求矩阵
int Allocation[50][50]= {0};     //分配矩阵
int Need[50][50]= {0};           //需求矩阵
int Work[50];                //工作向量
int Finish[50];              //状态标志
int Request[50][50];         //进程申请资源向量
int list[50];
int rnum,pnum;
void putin()
{
    cout<<"请输入资源的数量:";
    cin>>rnum;
    cout<<"请输入进程的数量:";
    cin>>pnum;
    cout<<"请依次输入"<<rnum<<"种资源的数量"<<endl;
    for(int i=1; i<=rnum; i++)
    {
        cout<<"第"<<i<<"类资源:";
        cin>>Available[i];
    }
    cout<<"请依次输入"<<pnum<<"个进程的所需最大资源数量"<<endl;
    for(int i=1; i<=pnum; i++)
    {
        for(int j=1; j<=rnum; j++)
        {
            cout<<"第"<<i<<"个进程所需第"<<j<<"种资源数量:";
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
        cout<<"安全序列为:";
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
        cout<<"无安全序列"<<endl;
        for(int i=1;i<=rnum;i++)
        {
            Allocation[tp][i]-=Request[tp][i];
            Need[tp][i]+=Request[tp][i];
        }
    }
}

void request()
{
    cout<<"请输入发出申请的进程号:";
    int tp;
    cin>>tp;
    if(tp>pnum)
    {
        cout<<"没有这个进程"<<endl;
        return;
    }
    cout<<"请输入该进程对各类资源申请的数量"<<endl;
    for(int i=1; i<=rnum; i++)
    {
        cout<<"第"<<i<<"类资源需求:";
        int mark = 0;
        int t;
        while(mark==0)
        {
            cin>>t;
            if(t>Need[tp][i])
            {
                cout<<"申请数量不能大于需求量"<<endl;
                mark=0;
            }
            else if(t>Available[i])
            {
                cout<<"申请数量不能大于可获得量"<<endl;
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
    cout<<"进程号              最大需求               现需求                 已分配"<<endl;
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
    cout<<"现系统资源剩余量:";
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
    cout<<"退出输入e,查看当前状态输入a,发出请求输入r"<<endl;
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
        cout<<"退出输入e,查看当前状态输入a,发出请求输入r"<<endl;
        cin>>key;
    }
    return 0;
}
