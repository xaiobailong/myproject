#include "view_talkgroup.h"
#include "net_h.h"
#include "online.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

extern map<string, int> online;

void view_talkgroup::process(Json::Value val, int cli_fd)
{
	cout << val << endl;
	_reason = val["reason"].asString();
	map<string, int>::iterator it = online.begin();
	for ( ; it != online.end(); ++it )
	{
		if ( it->second != cli_fd )
		{
			_cli_fd = it->second;
			response();
		}
	}
}

void view_talkgroup::response()
{
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
