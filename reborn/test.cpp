#include <iostream>
#include <boost/any.hpp>
#include "boo.h"

using namespace boo;

void symbol_test()
{
	// std::cout << "### test2 ###" << std::endl;

	boost::any x = symbol::get("bar");
	std::cout << boost::any_cast<symbol*>(x)->to_s() << std::endl;
	std::cout << boost::any_cast<symbol*>(x)->inspect() << std::endl;
        
    // std::cout << "HERE #1" << std::endl;

    x = symbol::get("hoge");
	std::cout << boost::any_cast<symbol*>(x)->to_s() << std::endl;
	std::cout << boost::any_cast<symbol*>(x)->inspect() << std::endl;
    
    // std::cout << "HERE #2" << std::endl;

	x = symbol::get("bar");
	std::cout << boost::any_cast<symbol*>(x)->to_s() << std::endl;
	std::cout << boost::any_cast<symbol*>(x)->inspect() << std::endl;
}

void boolean_test()
{
	std::cout << TRUE()->to_s() << std::endl;
	std::cout << FALSE()->to_s() << std::endl;
	std::cout << BOOLEAN(true)->to_s() << std::endl;
	std::cout << BOOLEAN(false)->to_s() << std::endl;
}

void pair_test()
{
	symbol* foo = symbol::get("foo");
	symbol* bar = symbol::get("bar");
	symbol* hoge = symbol::get("hoge");
	symbol* dona = symbol::get("dona");

	object* p = cons(foo, bar);
	std::cout << p->to_s() << std::endl;
	std::cout << car(p)->to_s() << std::endl;
	std::cout << cdr(p)->to_s() << std::endl;
	setcar(p, hoge);
	setcdr(p, dona);
	std::cout << p->to_s() << std::endl;
	p = cons(bar, p);
	std::cout << p->to_s() << std::endl;

	p = cons(foo, NIL());
	std::cout << p->to_s() << std::endl;

	std::cout << is_null(bar) << std::endl;
	std::cout << is_null(NIL()) << std::endl;
}

int main()
{
	symbol_test();
	boolean_test();
	pair_test();
}
