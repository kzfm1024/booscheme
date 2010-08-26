all:
	g++ -I/usr/local/boost_1_44_0 -D_BOOSCHEME_DEBUG_ booscheme.cpp environment.cpp

clean:
	rm -f a.out
