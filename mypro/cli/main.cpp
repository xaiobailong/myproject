#include"fun.h"
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;

int main()
{
	//判断参数
	/*if ( argc < 3 )
	{
		cerr << "arg  not enought;error:" << errno << endl;
		exit(1);
	}*/
	

	//获取ip,port
	/*char *ip = argv[1];
	unsigned short port = 0;
	for (int i = strlen(argv[2]) - 1; i >= 0; --i)
	{
		port = port *10 + ( argv[2][i] - '0' );
	}*/

	char ip[] = "127.0.0.1";

	run(ip, 6000);

	return 0;
}
