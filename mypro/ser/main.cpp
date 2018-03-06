#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <map>
#include "online.h"
#include <string>
#include "tcpsever.h"
using namespace std;

int main()
{
	//ip  port  启动线程个数
	/*if(argc < 4)
	{
		cerr << "arg not enought;errno:" << errno << endl;
		exit(1);
	}

	//ip  port  pth_num
	unsigned short port = 0;
	for ( int i = strlen(argv[2]) - 1; i >= 0; --i )
	{
		port = port * 10 + ( argv[2][i] - '0' );
	}

	int pth_num = argv[3][0] - '0'; */
	//构造服务器对象
	char str[] = "127.0.0.1";
	tcpsever ser(str, 6000, 4);

	//运行服务器
	ser.run();

	return 0;
}
