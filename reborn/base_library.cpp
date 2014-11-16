#include <limits.h> // INT_MAX
#include "boo.h"
#include "boo_types.h"
#include "base_library.h"

static int UNLIMITED = INT_MAX;

namespace boo
{
    void base_library::install(environment* env)
	{
		env->define(symbol::get("cons"), new primitive(cons, 2));
		env->define(symbol::get("car"),  new primitive(car, 1)); // boo::car()
		env->define(symbol::get("car"),  new primitive(cdr, 1)); // boo::cdr()

		env->define(symbol::get("+"),  new primitive(plus, 0, UNLIMITED));
		env->define(symbol::get("*"),  new primitive(times, 0, UNLIMITED));
	}

	object* base_library::cons(object* args)
	{
		return boo::cons(first(args), second(args));
	}

	object* base_library::plus(object* args)
	{
		number* n1 = dynamic_cast<number*>(first(args));
		number* n2 = dynamic_cast<number*>(second(args));
		return new number(n1->to_i() + n2->to_i());
	}

	object* base_library::times(object* args)
	{
		number* n1 = dynamic_cast<number*>(first(args));
		number* n2 = dynamic_cast<number*>(second(args));
		return new number(n1->to_i() * n2->to_i());
    }
}