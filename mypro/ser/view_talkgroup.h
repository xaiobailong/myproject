#ifndef VIEW_TALKONE_H
#define VIEW TALKONE_H

#include "view.h"

class view_talkgroup : public view
{
	public:
		void process(Json::Value val, int cli_fd);
		void response();
	private:
		int _cli_fd;
		string _reason;
};

#endif
