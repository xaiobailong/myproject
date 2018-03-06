#ifndef VIEW_REGISTER_H
#define VIEW_REGISTER_H

#include"view.h"

class view_register : public view
{
	public:
		void process(Json::Value val,int cli_fd);
		void response();

	private:
		int _cli_fd;
		string _reason;

};

#endif
