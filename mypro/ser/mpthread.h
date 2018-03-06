#ifndef MTHREAD_H
#define MTHREAD_H

#include<iostream>
#include"control.h"
#include<event.h>
#include<string>
#include<map>
using namespace std;

typedef class mpthread
{
	public:
		mpthread(int sock_1);
		~mpthread();
		void run();
	private:
		struct event_base* _base;//libevent
		int _sock_1;//socketpair  1
		map<int,struct event*>  _event_map;//保存事件的map
		Pcontrol _Mcontrol;//控制台
		
		friend  void *pthread_run(void *arg);
		friend void cli_cb(int fd, short event, void *arg);
		friend void sock_1_cb(int fd, short event, void *arg);

}Mpthread,*Pmpthread;

#endif
