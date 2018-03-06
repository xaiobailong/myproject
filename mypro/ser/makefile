all:main
main:main.o tcpsever.o mpthread.o control.o view_register.o view_login.o user.o view_getlist.o view_talkone.o view_talkgroup.o view_exit.o
	g++ -o main main.o tcpsever.o mpthread.o control.o view_register.o view_login.o user.o view_getlist.o view_talkone.o view_talkgroup.o view_exit.o -pthread -levent -ljson -lmysqlclient
main.o:main.cpp tcpsever.h net_h.h online.h
	g++ -c main.cpp
tcpsever.o:tcpsever.cpp tcpsever.h mpthread.h net_h.h
	g++ -c tcpsever.cpp
mpthread.o:mpthread.cpp mpthread.h control.h net_h.h
	g++ -c mpthread.cpp
control.o:control.cpp control.h public.h view_register.h view_login.h view_talkone.h view_talkgroup.h view_getlist.h view_exit.h
	g++ -c control.cpp
view_register.o:view_register.cpp view_register.h user.h view.h net_h.h
	g++ -c view_register.cpp
user.o:user.cpp user.h 
	g++ -c user.cpp
view_login.o:view_login.cpp view_login.h user.h view.h net_h.h tcpsever.cpp
	g++ -c view_login.cpp
view_getlist.o:view_getlist.cpp view_getlist.h user.h view.h net_h.h
	g++ -c view_getlist.cpp
view_talkone.o:view_talkone.cpp view_talkone.h user.h view.h net_h.h
	g++ -c view_talkone.cpp
view_talkgroup.o:view_talkgroup.cpp view_talkgroup.h user.h view.h net_h.h
	g++ -c view_talkgroup.cpp
view_exit.o:view_exit.cpp view_exit.h user.h view.h net_h.h
	g++ -c view_exit.cpp
clean:
	rm -f *.o main
