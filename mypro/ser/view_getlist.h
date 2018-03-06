#ifndef VIEW_GETLIST_H
#define VIEW_GETLIST_H

#include"view.h"

class view_getlist : public view
{
	public:
		void process(Json::Value val,int cli_fd);
		void response();
	private:
		int _cli_fd;
		string _reason;
};

#endif

