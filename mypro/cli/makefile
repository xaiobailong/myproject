all:main
main:main.o fun.o
	g++ -o main main.o fun.o -pthread -ljson
main.o:main.cpp fun.h
	g++ -c main.cpp
fun.o:fun.cpp fun.h public.h
	g++ -c fun.cpp
clean:
	rm -f *.o main
