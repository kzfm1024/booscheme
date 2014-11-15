#include <iostream>
#include <boost/any.hpp>

#include "boo.h"
#include "boolean.h"
#include "null.h"
#include "pair.h"
#include "number.h"
#include "string_t.h"
#include "character.h"
#include "symbol.h"
#include "misc.h"
#include "environment.h"
#include "primitive.h"
#include "closure.h"
#include "output_port.h"
#include "input_port.h"

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
	print("p = cons(foo, bar)", p);
	print("car(p)", car(p));
	print("cdr(p)", cdr(p));
	setcar(p, hoge);
	setcdr(p, dona);
	print(p);
	p = cons(bar, p);
	print(p);

	p = cons(foo, NIL());
	print(p);

	std::cout << is_null(bar) << std::endl;
	std::cout << is_null(NIL()) << std::endl;
}

void number_test()
{
	number* num = new number(188);
	print(num);
}

void string_test()
{
	output_port* out = new output_port(std::cout);
	string_t* str = new string_t("foo");
	write(str, out, true); out->newline();
	write(str, out, false); out->newline();
	out->flush();
	print(out);
}

void character_test()
{
	output_port* out = new output_port(std::cout);
	character* ch = new character('y');
	write(ch, out, true); out->newline();
	write(ch, out, false); out->newline();
	out->flush();
	print(out);
}

void environment_test()
{
	symbol* x = symbol::get("x");
	symbol* y = symbol::get("y");
	symbol* z = symbol::get("z");
	symbol* xx = symbol::get("xx");

	number* i = new number(123);
	number* j = new number(456);
	number* k = new number(789);

	pair* vars = cons(x, cons(y, cons(z, NIL())));
	pair* vals = cons(i, cons(j, cons(k, NIL())));

	environment* env = new environment(vars, vals, 0);
	print(env->lookup(x));
	print(env->lookup(y));
	print(env->lookup(z));

	try
	{
		print(env->lookup(xx));	
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void eval_test()
{
	symbol* plus = symbol::get("+");
	primitive* plus_proc = new primitive(0, 2, 2);

	symbol* x = symbol::get("x");
	symbol* y = symbol::get("y");
	symbol* z = symbol::get("z");

	number* i = new number(123);
	number* j = new number(456);
	number* k = new number(789);

	pair* vars = cons(plus, cons(x, cons(y, cons(z, NIL()))));
	pair* vals = cons(plus_proc, cons(i, cons(j, cons(k, NIL()))));

	environment* env = new environment(vars, vals, 0);

	pair* exp = cons(plus, (cons(x, (cons(y, NIL())))));
	print(eval(exp, env));

	pair* params = cons(x, NIL());
	pair* body = cons(plus, cons(x, cons(x, NIL())));
	closure* f = new closure(params, body, env);

	exp = cons(f, cons(new number(5), NIL()));
	print(eval(exp, env));
}

void scm_test()
{
	symbol* plus = symbol::get("+");
	primitive* plus_proc = new primitive(0, 2, 2);

	symbol* x = symbol::get("x");
	symbol* y = symbol::get("y");
	symbol* z = symbol::get("z");

	number* i = new number(123);
	number* j = new number(456);
	number* k = new number(789);

	pair* vars = cons(plus, cons(x, cons(y, cons(z, NIL()))));
	pair* vals = cons(plus_proc, cons(i, cons(j, cons(k, NIL()))));

	environment* env = new environment(vars, vals, 0);

	input_port* in = new input_port(std::cin);
	output_port* out = new output_port(std::cout);
	std::cout << "boo> " << std::flush;
	write(eval(in->read(), env), out, true);
	std::cout << std::endl;
}

int main()
{
#if 0
	symbol_test();
	boolean_test();
	pair_test();
	number_test();

	string_test();
	character_test();
	environment_test();
	eval_test();
#endif

	scm_test();
}
