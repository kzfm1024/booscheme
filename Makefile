INCLUDES = -I/usr/local/boost_1_44_0
CPPFLAGS = -g -D_BOOSCHEME_DEBUG_

all:
	g++ ${INCLUDES} ${CPPFLAGS} -c utils.cpp
	g++ ${INCLUDES} ${CPPFLAGS} -c input_port.cpp
	g++ ${INCLUDES} ${CPPFLAGS} -c environment.cpp
	g++ ${INCLUDES} ${CPPFLAGS} -c test.cpp
	g++ ${INCLUDES} ${CPPFLAGS} test.o environment.o utils.o

clean:
	rm -f a.out *.o *~
	rm -rf a.out.dSYM
