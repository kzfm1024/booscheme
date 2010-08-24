all:
	g++ -I/usr/local/boost_1_44_0 booscheme.cpp environment.cpp

clean:
	rm -f a.out
