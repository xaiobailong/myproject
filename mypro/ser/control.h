#ifndef CONTROL_H
#define CONTROL_H

#include<iostream>
#include"view.h"
#include<map>
using namespace std;

typedef class control
{
	public:
		control();
		~control();
		void process(int fd, char *buff);
	private:
		map<int,view *> _model_map;//model
}Control,*Pcontrol;

#endif
