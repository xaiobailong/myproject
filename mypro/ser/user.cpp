#include "user.h"
#include <string.h>

void user_insert(Json::Value val)
{
	MYSQL mysql;
	MYSQL *connect;
	while(1)
	{
		mysql_init(&mysql);
		connect = mysql_real_connect(&mysql, "localhost", "root", "123456", "talk", 0, 0, 0);
		if (connect == NULL)
		{
			cout << mysql_error(&mysql) << endl;
		}
		else
		{
			break;
		}
	}
	char str[80] = "insert into user values('";
	strcat(str, val["name"].asString().c_str());
	strcat(str, "','");
	strcat(str, val["passwd"].asString().c_str());
	strcat(str, "')");
	
	mysql_query(connect, "set names utf8");
	mysql_query(connect, str);
	mysql_close(connect);
}

int user_find(Json::Value val)
{
	MYSQL mysql;
	MYSQL *connect;
	while(1)
	{
		mysql_init(&mysql);
		connect = mysql_real_connect(&mysql, "localhost", "root", "123456", "talk", 0, 0, 0);
		if (connect == NULL)
		{
			cout << mysql_error(&mysql) << endl;
		}
		else
		{
			break;
		}
	}
	char str[80] = "select * from user where name='";
	strcat(str, val["name"].asString().c_str());
	strcat(str, "'");

	mysql_query(connect, "set names utf8");
	mysql_query(connect, str);

	MYSQL_RES *res = mysql_store_result(connect);
	MYSQL_ROW row = mysql_fetch_row(res);
	if ( row == NULL )
		return 0;
	char pname[20] = { 0 };
	char pwd[20] = { 0 };
	strcpy(pname, (const char*)row[0]);
	strcpy(pwd, (const char *)row[1]);
	mysql_free_result(res);
	if (val["type"].asInt() != 1)//
		return 1;
	else
	{
		if ( strcmp(pwd, val["passwd"].asString().c_str()) == 0 )
			return 2;
		else
			return -1;
	}		
	mysql_close(connect);
}

char *get_name()
{
	MYSQL mysql;
	MYSQL *connect;
	while(1)
	{
		mysql_init(&mysql);
		connect = mysql_real_connect(&mysql, "localhost", "root", "123456", "talk", 0, 0, 0);
		if (connect == NULL)
		{
			cout << mysql_error(&mysql) << endl;
		}
		else
		{
			break;
		}
	}
	mysql_query(connect, "set names utf8");
	mysql_query(connect, "select * from user");

	MYSQL_RES *res = mysql_store_result(connect);
	MYSQL_ROW row = mysql_fetch_row(res);
	if ( row == NULL )
		return NULL;

	char *buff = new char[512];//存放所有注册用户名
	*buff = '\0';
	while(row != NULL)
	{
		strcat(buff, (const char*)row[0]);
		strcat(buff, "//");
		row = mysql_fetch_row(res);
	}
	
	mysql_free_result(res);
	mysql_close(connect);
	return buff;
}

void offline_insert(Json::Value val)
{

}

char *offline_find(char *name)
{
	
}
