#include"view_login.h"
#include"user.h"
#include"net_h.h"
#include"online.h"
#include<string>
#include<map>
#include<iostream>
using namespace std;

extern map<string, int> online;

void view_login::process(Json::Value val,int cli_fd)
{
	_cli_fd = cli_fd;
	//val['name']   
	//在数据库中查询name
	int n = user_find(val);
	if (n == 0 )
		_reason = "user no exist";
	else if(n == -1)
		_reason = "password false";
	else if (n == 2)
	{
		//查在线表
		map<string, int>::iterator it = online.find(val["name"].asString());
		if (it == online.end())//在线表中无此用户
		{
			online.insert(make_pair(val["name"].asString(), _cli_fd));
			_reason = "login success";
		}
		else
		{
			_reason = "user already online";
		}
		it = online.begin();
		for( ; it != online.end(); ++it)
		{
			cout << it->first << endl;
		}
	}	
	response();
}

void view_login::response()
{
	//send->fd   reason
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
