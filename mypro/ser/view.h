#ifndef VIEW_H
#define VIEW_H

#include<iostream>
#include<json/json.h>
using namespace std;

class view
{
	public:
		virtual void process(Json::Value val,int cli_fd)=0;
		virtual void response()=0;
};

#endif
