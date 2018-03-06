#include"view_register.h"
#include<mysql/mysql.h>
#include<sys/socket.h>
#include"user.h"

void view_register::process(Json::Value val, int cli_fd)
{
	_cli_fd = cli_fd;
	//先查user表，判断是否已经注册
	if ( user_find(val) == 1 )
	{
		//该用户已存在
		_reason = "user already exist";
	}
	else
	{
		user_insert(val);
		//注册成功
		_reason = "register success";
	}
	response();	
}

void view_register::response()
{
	send(_cli_fd, _reason.c_str(), _reason.size(), 0);
}
