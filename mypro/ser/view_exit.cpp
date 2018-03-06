#include"view_exit.h"
#include"user.h"
#include"net_h.h"
#include"online.h"
#include<string>
#include<map>
#include<iostream>
using namespace std;

extern map<string, int> online;

void view_exit::process(Json::Value val,int cli_fd)
{
	online.erase(val["name"].asString());
	_cli_fd = cli_fd;
	_reason = "exit success";
	response();
}

void view_exit::response()
{
	//send->fd   reason
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
