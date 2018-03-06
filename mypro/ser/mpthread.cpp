#include<iostream>
#include<json/json.h>
#include<pthread.h>
#include"net_h.h"
#include<errno.h>
#include<event.h>
#include<string.h>
#include<stdlib.h>
#include"mpthread.h"
#include"online.h"
using namespace std;

extern map<string, int> online;

void cli_cb(int fd, short event, void *arg)
{
	if ( EV_READ & event )
	{
	 	Pmpthread mthis = (Pmpthread)arg;
		//recv  ->buff
		char buff[128] = { 0 };
	
		if ( recv(fd, buff, 127,0) <= 0 )
		{
			event_del(mthis->_event_map[fd]);
			//不仅要抛出错误还要从保存事件中删除该套接字
			mthis->_event_map.erase(fd);
			close(fd);
			map<string, int>::iterator it = online.begin();
			for ( ; it != online.end(); ++it )
			{
				if (fd == it->second)
				{
					online.erase(it->first);
				}
			}
			cerr << "client over;error:" << errno << endl;
			return;
		}
		
		//buff->control
		(mthis->_Mcontrol)->process(fd, buff);
	}
}

void sock_1_cb(int fd, short event, void *arg)
{	
	if ( EV_READ & event )
	{
	 	Pmpthread mthis = (Pmpthread)arg;

		//recv  ->cli_fd
		char buff[6] = { 0 };
		if ( recv(fd, buff, 5, 0) < 0 )
		{
			cerr << "sock_1 read fail;error:" << errno << endl;
			return;
		}
		
		//将cli_fd加入到监听队列
		int clifd = atoi(buff);
		cout << clifd << endl;
	 	struct event *reason_ev =  event_new(mthis->_base, clifd, EV_READ | EV_PERSIST,cli_cb, mthis);
		if ( reason_ev == NULL )
		{
			cerr << "监听客户端套接字失败；error:" << errno << endl;
		}
		
		event_add(reason_ev, NULL);
	
			///将客户端事件保存
		mthis->_event_map.insert(make_pair(clifd, reason_ev));
		
		char buff_count[6] = { 0 };
		sprintf(buff_count, "%d", mthis->_event_map.size());
		//send(pth_event_num);//将线程目前的监听量发送给主线程
		send(mthis->_sock_1, buff_count, 5, 0);
	}
}


void *pthread_run(void *arg)
{
	Pmpthread mthis = (Pmpthread)arg;
	mthis->_base = event_init();

	//将sock_1加入到监听队列 -》libevent
	struct event *sock_1_event = event_new(mthis->_base,mthis->_sock_1,EV_READ|EV_PERSIST,sock_1_cb,mthis);
	if ( sock_1_event == NULL )
	{
		cerr << "sock_1 fail;error:" << errno << endl;
	}
	
	event_add(sock_1_event,NULL);
	event_base_dispatch(mthis->_base);
}

mpthread::~mpthread()
{

}


mpthread::mpthread(int sock_1)
{
	_sock_1 = sock_1;
	_Mcontrol = new control();
	//建立线程
	pthread_t pid;
	pthread_create(&pid, NULL, pthread_run, this);
}

