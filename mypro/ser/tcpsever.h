#ifndef TCPSEVER_H
#define TCPSEVER_H

#include<iostream>
#include<event.h>
#include<vector>
#include<map>
using namespace std;

class sock
{
	public:
		sock(int sv[2])
		{
			_sv[0] = sv[0];
			_sv[1] = sv[1];
		}
		~sock()
		{
	
		}
		int _sv[2];
};

typedef class tcpsever
{
	public:
		tcpsever(char *ip,unsigned short port,int pth_num);
		~tcpsever();
		void run();

		void create_socket_pair();
		void create_pth_base();
	private:
		struct event_base* _base;//libevent
		int _listen_fd;//监听套接字
		int _pth_num;//线程个数
		vector<sock> _socket_pair_base;//socket_pair
		map<int, int> _pth_map;//线程压力统计
	
		friend void listen_cb(int fd,short event,void *arg);
		friend void socket_0_cb(int fd,short event,void *arg);
}tcpsever,*Ptcpsever;

#endif
