#ifndef VIEW_LOGIN_H
#define VIEW_LOGIN_H

#include"view.h"

class view_login : public view
{
	public:
		void process(Json::Value val,int cli_fd);
		void response();
	private:
		int _cli_fd;
		string _reason;
};
#endif
