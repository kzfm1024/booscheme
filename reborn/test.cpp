#include <iostream>
#include <boost/any.hpp> // TO BE DELETED

#include "boo.h"
#include "boo_types.h"
#include "base_library.h"

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
	pdebug("p = cons(foo, bar)", p);
	pdebug("car(p)", car(p));
	pdebug("cdr(p)", cdr(p));
	setcar(p, hoge);
	setcdr(p, dona);
	pdebug(p);
	p = cons(bar, p);
	pdebug(p);

	p = cons(foo, NIL());
	pdebug(p);

	std::cout << is_null(bar) << std::endl;
	std::cout << is_null(NIL()) << std::endl;
}

void number_test()
{
	number* num = new number(188);
	pdebug(num);
}

void string_test()
{
	output_port* out = new output_port(std::cout);
	string_t* str = new string_t("foo");
	write(str, out, true); out->newline();
	write(str, out, false); out->newline();
	out->flush();
	pdebug(out);
}

void character_test()
{
	output_port* out = new output_port(std::cout);
	character* ch = new character('y');
	write(ch, out, true); out->newline();
	write(ch, out, false); out->newline();
	out->flush();
	pdebug(out);
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

	object* vars = cons(x, cons(y, cons(z, NIL())));
	object* vals = cons(i, cons(j, cons(k, NIL())));

	environment* env = new environment(vars, vals, 0);
	pdebug(env->lookup(x));
	pdebug(env->lookup(y));
	pdebug(env->lookup(z));

	try
	{
		pdebug(env->lookup(xx));	
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

	object* vars = cons(plus, cons(x, cons(y, cons(z, NIL()))));
	object* vals = cons(plus_proc, cons(i, cons(j, cons(k, NIL()))));

	environment* env = new environment(vars, vals, 0);

	object* exp = cons(plus, (cons(x, (cons(y, NIL())))));
	pdebug(eval(exp, env));

	object* params = cons(x, NIL());
	object* body = cons(plus, cons(x, cons(x, NIL())));
	closure* f = new closure(params, body, env);

	exp = cons(f, cons(new number(5), NIL()));
	pdebug(eval(exp, env));
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

	object* vars = cons(plus, cons(x, cons(y, cons(z, NIL()))));
	object* vals = cons(plus_proc, cons(i, cons(j, cons(k, NIL()))));

	environment* env = new environment(vars, vals, 0);

	input_port* in = new input_port(std::cin);
	output_port* out = new output_port(std::cout);
	std::cout << "boo> " << std::flush;
	write(eval(in->read(), env), out, true);
	std::cout << std::endl;
}

void scm_test2()
{
	environment* env = new environment(NIL(), NIL(), 0);
	base_library::install(env);

	input_port* in = new input_port(std::cin);
	output_port* out = new output_port(std::cout);

	while (true) 
	{
		std::cout << "boo> " << std::flush;
		write(eval(in->read(), env), out, true);
		std::cout << std::endl;
	}
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

	// scm_test();
	scm_test2();
}
