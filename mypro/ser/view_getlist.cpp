#include"view_getlist.h"
#include"net_h.h"
#include"user.h"
#include<iostream>
using namespace std;

void view_getlist::process(Json::Value val, int cli_fd)
{
	_cli_fd = cli_fd;
	char *buff = get_name();
	if (buff == NULL)
		_reason = "no user";
	_reason = buff;

	cout << buff << endl;
	delete buff;
	response();

}

void view_getlist::response()
{
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
