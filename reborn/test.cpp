#include <iostream>
#include <boost/any.hpp>
#include "symbol.h"
using namespace booscheme;

void test()
{
	std::cout << "### test2 ###" << std::endl;

	boost::any x = symbol::get("bar");
	std::cout << boost::any_cast<symbol*>(x)->name() << std::endl;
	std::cout << boost::any_cast<symbol*>(x)->inspect() << std::endl;

    std::cout << "HERE #1" << std::endl;

    x = symbol::get("hoge");
	std::cout << boost::any_cast<symbol*>(x)->name() << std::endl;
	std::cout << boost::any_cast<symbol*>(x) << std::endl;
    
    std::cout << "HERE #2" << std::endl;

	x = symbol::get("bar");
	std::cout << boost::any_cast<symbol*>(x)->name() << std::endl;
	std::cout << boost::any_cast<symbol*>(x)->inspect() << std::endl;
}

int main()
{
	test();
}
