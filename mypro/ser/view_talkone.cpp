#include "view_talkone.h"
#include "net_h.h"
#include <string>
#include <map>
#include <iostream>
#include "online.h"
using namespace std;

extern map<string, int> online;

void view_talkone::process(Json::Value val, int cli_fd)
{
	map<string, int>::iterator it = online.begin();
	for (; it != online.end(); ++it)
	{
		cout << it->first << endl;
	}
	it = online.find(val["name"].asString());
	cout << val << endl;
	if (it != online.end())
	{
		cout << "success" << endl;
		_reason = val["reason"].asString();
		cout << _reason << endl;
		_cli_fd = it->second;
		response();
	}
	else
	{
		cout << "error" << endl;
		//存放到离线表
	}
}

void view_talkone::response()
{
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
