#include"public.h"
#include"fun.h"
#include<errno.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<json/json.h>
#include<iostream>
using namespace std;

void *read_cb(void *arg)//处理socketfd上的读事件
{
	int fd = (int)arg;
	while(1)
	{
		char buff[512] = {0};
		if ( recv(fd, buff, 511, 0) > 0 )
		{
			cout << buff << endl;
		}
		else
		{
			cerr << "ser fault;error:" << errno << endl;
			close(fd);
			cout << "sever fail" << endl;
			exit(1);
		}
	}
}

void talkone(int fd, string &ownname)//一对一聊天
{
	string name;
	cout << "please cin acceptor name : ";
	cin >> name;

	string reason;
	cout << "please cin reason : ";
	cin >> reason;

	//json 打包数据并发送,
	Json::Value val;
	val["type"] = MSG_TYPE_TALKONE;
	val["name"] = name;
	reason = reason + "\nsender:";
	reason = reason + ownname;
	val["reason"] = reason;
	
	//将自己的名字也发送过去，让对方知道是谁发送的消息

	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
}

void talkgroup(int fd, string &ownname)//群发消息
{
	string reason;
	cout << "please cin reason" << endl;
	cin >> reason;

	//json 打包数据并发送,
	Json::Value val;
	val["type"] = MSG_TYPE_TALKGROUP;
	reason = reason + "\n/";
	reason = reason + ownname;
	val["reason"] = reason;

	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
}

void exit_last(int fd, string &ownname)//退到上一层
{
	Json::Value val;
	val["type"] = MSG_TYPE_EXIT;
	val["name"] = ownname;

	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
}

void getlist(int fd)//获取注册列表
{
	Json::Value val;
	val["type"] = MSG_TYPE_GETLIST;

	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
}

void run(char *ip,unsigned short port)
{
	//连接服务器
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		cerr << "clientfd creat fail;error:" << errno << endl;
		//exit(1);
		return;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	int res = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
	if ( res == -1 )
	{
		cerr << "connect fail;error:" << errno << endl;
		//exit(1);
		return;
	}
	
	while(1)
	{
		cout<<"1.register"<<endl;
		cout<<"2.login"<<endl;
		cout<<"3.exit"<<endl;
		int chioce;
		cout << "please cin chioce: " << endl;
		cin>>chioce;
		switch(chioce)
		{
			case 1:
				{
					Register(fd);
				}break;
			case 2:
				{
					login(fd);
				}break;
			case 3:
				{
					exit(1);
				}break;
			default:
				{
					cout << "no chioce,please try agin" << endl;
				}
		}
	}
}

void Register(int fd)
{
	string name;
	string pw;

	cout << "please cin name:" << endl;
	cin >> name;

	cout << "please cin pw:" << endl;
	cin >> pw;
	
	//用json打包type  name   pw
	Json::Value val;
	val["type"] = MSG_TYPE_REGISTER;
	val["name"] = name;
	val["passwd"] = pw;

	//给服务器发送json包
	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);

	//接受服务器的返回
	char buff[20] = { 0 };
	recv(fd, buff, 19, 0);

	//判断注册是否成功
	cout << buff <<endl;
}

void login(int fd)
{
	string name;
	string pw;
	
	cout << "please cin name:" <<endl;
	cin >> name;
	cout << "please cin pw:" << endl;
	cin >> pw;

	//用json打包type  name   pw
	Json::Value val;
	val["type"] = MSG_TYPE_LOGIN;
	val["name"] = name;
	val["passwd"] = pw;
	

	//给服务器发送json包
	send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);

	//接受服务器的返回
	char buff[16] = { 0 };
	recv(fd, buff, 15, 0);
	cout << buff << endl;
	//判断注册是否成功
	if( strcmp(buff, "login success") == 0 )
	{
		//启动一个线程（只负责接否服务器端的消息，然后输出）
		pthread_t id;
		pthread_create(&id, NULL, read_cb, (void *)fd); 

		while(1)
		{			
			cout << "1.talkone" << endl;
			cout << "2.talkgroup" << endl;
			cout << "3.getlist" << endl;
			cout << "4.exit" << endl;
			int chioce;
			cout << "cin a choice:" << endl;
			cin >> chioce;
				
			int flags = 0;//退到上一层的标志

			switch(chioce)
			{
				case 1:
					{	
						talkone(fd, name);
					}break;
				case 2:
					{
						talkgroup(fd, name);
					}break;
				case 3:
					{
						getlist(fd);
					}break;
				case 4:
					{
						exit_last(fd, name);
						flags = 1;
					}break;
				default:
					{
						cout << "no chioce, please try again" << endl;
					}
			
			}
			if (flags == 1)
				break;
		}
	}
	else
	{
		cout<<"login fail;"<<endl;
	}
}

