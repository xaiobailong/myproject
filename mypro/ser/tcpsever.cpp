#include <iostream>
#include "net_h.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <map>
#include <json/json.h>
#include <stdlib.h>
#include "mpthread.h"
#include "tcpsever.h"
using namespace std;

tcpsever::tcpsever(char *ip,unsigned short port,int pth_num)
{
	_pth_num = pth_num;

	///创建服务器
	int listen_fd  = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1)
	{
		cerr << "ser_fd create fail;error:" << errno << endl;
		return;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	if ( -1 == bind(listen_fd, (struct sockaddr*)&saddr, sizeof(saddr)) ) 
	{
		cerr << "ser_fd bind fail;error:" << errno << endl;
		return;
	}

	if ( -1 == listen(listen_fd, 128) )
	{
		cerr << "serfd listen fail;error:" << errno << endl;
		return;
	}
	
	_listen_fd = listen_fd;


	//初始化libevent
	_base = event_init();	
}

tcpsever::~tcpsever()
{

}

void listen_cb(int fd,short event,void *arg)
{
	if ( EV_READ & event )
	{
		//mthis = (Ptcpsever)arg;
		Ptcpsever mthis = (Ptcpsever)arg;
		struct sockaddr_in caddr;
		socklen_t len = sizeof(caddr);

		//aeccpt ->cli_fd
		int c = accept(fd, (struct sockaddr*)&caddr,&len);
		if ( c < 0 )
		{
			cerr << "client connect fail;error:" << errno << endl;
			exit(1);
			//return;
		}
	
		cout << "cli_fd : " << c << endl;		
		//找到线程压力最小的子线程
		
		char buff[6] = { 0 }; 
		sprintf(buff, "%d", c);
		send(mthis->_socket_pair_base[0]._sv[0], buff, 5, 0);
	}
		
}

void tcpsever::run()
{
	//创建socket_pair
	create_socket_pair();

	//启动子线程
	create_pth_base();

	//将_listen_fd加入到libevent   ->listen_cb()
	struct event* listen_ev = event_new(_base, _listen_fd, EV_READ | EV_PERSIST, listen_cb, (void *)this);
	if ( listen_ev == NULL )
	{
		cerr << "listen fail;error:" << errno << endl;
	}
	event_add(listen_ev, NULL);
	event_base_dispatch(_base);
}

void socket_0_cb(int fd,short event,void *arg)
{
	if (EV_READ & event)
	{
		//map强转
		Ptcpsever mthis = (Ptcpsever)arg;
		//recv  ->pth_event_num
		char buff[6] = { 0 };
		if ( recv(fd, buff, 5, 0) < 0 )
		{
			cerr << "sock 0 fail;error:" << errno << endl;
			exit(1);
		}
		else
		{
			//update  ->   map
			int num = atoi(buff);
			cout << "num : " << num << endl;
			mthis->_pth_map[fd] = num;
		}
	}
}

void tcpsever::create_socket_pair()
{
	int sv[2] = {0, 0};

	for(int i = 0; i < _pth_num - 1; i++)
	{
		//socketpair(int arr[2])
		if ( -1 == socketpair(AF_UNIX, SOCK_STREAM, 0, sv) )
		{
			cerr << "sockpair fail;error:" << errno <<endl;
		}
		
		//将socketpair放入vector
		_socket_pair_base.push_back(sv);

		//将socketpair1端加入到_map
		_pth_map.insert(make_pair(sv[1], 0));


		//将socketpair  0 端监听起来
		//加入到libevent ->socket_0_cb()
		struct event *child_ev = event_new(_base, sv[0], EV_READ | EV_PERSIST, socket_0_cb, this);
		if ( child_ev == NULL )
		{
			cerr << "event create fail;error:" << errno << endl;
		}
		event_add(child_ev, NULL);
	}
}

void tcpsever::create_pth_base()
{
	for(int i = 0; i< _pth_num - 1; i++)
	{
		Pmpthread pth = new Mpthread(_socket_pair_base[i]._sv[1]);
	}
}
