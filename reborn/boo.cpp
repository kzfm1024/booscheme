//
// boo.cpp
//

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h> // FIXME: strtol 
#include <errno.h> // FIXME
#include <limits.h> // FIXME
#include <assert.h>
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
#include "procedure.h"
#include "closure.h"
#include "primitive.h"
#include "output_port.h"
#include "eof.h"

namespace boo
{
	boolean* TRUE()
	{
		static boolean* t;
		if (!t) t = new boolean(true);
		return t;
	}

	boolean* FALSE()
	{
		static boolean* f;
		if (!f) f = new boolean(false);
		return f;
	}

	boolean* BOOLEAN(bool x)
	{
		return x ? TRUE() : FALSE();
	}

	null* NIL()
	{
		static null* nil;
		if (!nil) nil = new null();
		return nil;
	}

	eof* EOF()
	{
		static eof* e;
		if (!e) e = new eof();
		return e;
	}

	misc* UNDEF()
	{
		static misc* undef;
		if (!undef) undef = new misc("#<undef>");
		return undef;
	}

	misc* error(const std::string& message)
	{
		throw std::runtime_error("*** ERROR: " + message);
		return new misc("#<error>");
	}

	misc* warn(const std::string& message)
	{
		std::cerr << "*** WARNING: " << message << std::endl;
		return new misc("#<warn>");
	}

#if 0
	pair* cons(object* a, object* b)
	{
		return new pair(a, b);
	}
#else
	object* cons(object* a, object* b)
	{
		return new pair(a, b);
	}
#endif

	object* car(object* x)
	{
		pair* p = dynamic_cast<pair*>(x);
		assert(p);
		return p->car();
	}

	object* cdr(object* x)
	{
		pair* p = dynamic_cast<pair*>(x);
		assert(p);
		return p->cdr();
	}

	object* setcar(object* x, object* a)
	{
		pair* p = dynamic_cast<pair*>(x);
		assert(p);
		return p->setcar(a);
	}

	object* setcdr(object* x, object* b)
	{
		pair* p = dynamic_cast<pair*>(x);
		assert(p);
		return p->setcdr(b);
	}

	object* first(object* x)
	{
		return car(x);
	}

	object* second(object* x)
	{
		return car(cdr(x));
	}

	object* third(object* x)
	{
		return car(cdr(cdr(x)));
	}

	object* list(object* a, object* b)
	{
		return cons(a, cons(b, NIL()));
	}

	int length(object* x)
	{
		int len = 0;
		while (is_pair(x))
		{
			len++;
			x = cdr(x);
		}
		return len;
	}

	bool truth(object* x)
	{
		boolean* b = dynamic_cast<boolean*>(x);
		return (b == FALSE()) ? false : true;
	}

	bool is_null(object* x)
	{
		null* nil = dynamic_cast<null*>(x);
		return nil ? true : false;
	}

	bool is_pair(object* x)
	{
		pair* p = dynamic_cast<pair*>(x);
		return p ? true : false;
	}

	bool is_number(object* x)
	{
		number* num = dynamic_cast<number*>(x);
		return num ? true : false;
	}

	bool is_string(object* x)
	{
		string_t* str = dynamic_cast<string_t*>(x);
		return str ? true : false;
	}

	bool is_character(object* x)
	{
		character* ch = dynamic_cast<character*>(x);
		return ch ? true : false;
	}

	bool is_symbol(object* x)
	{
		symbol* sym = dynamic_cast<symbol*>(x);
		return sym ? true : false;
	}

	bool is_symbol(object* x, const std::string& s)
	{
		symbol* sym = dynamic_cast<symbol*>(x);
		return (sym && sym->name() == s) ? true : false;
	}

	bool is_procedure(object* x, const std::string& s)
	{
		procedure* proc = dynamic_cast<procedure*>(x);
		return (proc && proc->name() == s) ? true : false;
	}

	bool is_closure(object* x)
	{
		closure* c = dynamic_cast<closure*>(x);
		return c ? true : false;
	}

	bool is_eof(object* x)
	{
		eof* e = dynamic_cast<eof*>(x);
		return e ? true : false;
	}

	number* to_number(const std::string& s)
	{
		long int val;
		char* endptr;
		int base = 10;
            
		val = strtol(s.c_str(), &endptr, base);

		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)))
		{
			throw std::invalid_argument("out of ranage");
		}
            
		if (*endptr)
		{
			throw std::invalid_argument("not a number");
		}

		return new number(val);
	}

	static void stringify_pair(pair* p, bool quoted, std::ostringstream& buf);

	static void stringify(object* x, bool quoted, std::ostringstream& buf)
	{
		if (is_pair(x))
		{
			stringify_pair(dynamic_cast<pair*>(x), quoted, buf);
		}
		else if (is_string(x)) 
		{
			if (quoted) buf << "\"";
			buf << x->to_s();
			if (quoted) buf << "\"";
		}
		else if (is_character(x))
		{
			if (quoted) buf << "#\\"; // buf.append("#\\");
			buf << x->to_s();
		}
		else
		{
			buf << x->to_s();
		}
	}

	static void stringify_pair(pair* p, bool quoted, std::ostringstream& buf)
	{
		std::string special;

		if (is_pair(p->cdr()) && is_null(cdr(p->cdr())))
		{
			if      (is_symbol(p->car(), "quote"))            { special = "'";  }
			else if (is_symbol(p->car(), "quasiquote"))       { special = "`";  }
			else if (is_symbol(p->car(), "unquote"))          { special = ",";  }
			else if (is_symbol(p->car(), "unquote-splicing")) { special = ",@"; }
		}

		if (special.size())
		{
			buf << special;
			stringify(car(p->cdr()), quoted, buf);
		}
		else
		{
			buf << "(";
			stringify(p->car(), quoted, buf);

			object* tail = p->cdr();
			while (is_pair(tail))
			{
				buf << " ";
				stringify(car(tail), quoted, buf);
				tail = cdr(tail);
			}

			if (!is_null(tail))
			{
				buf << " . ";
				stringify(tail, quoted, buf);
			}

			buf << ")";
		}
	}

	std::string stringify(object* x, bool quoted)
	{
		std::ostringstream buf;
		stringify(x, quoted, buf);
		return buf.str();
	}

	std::string stringify(object* x)
	{
		return stringify(x, true);
	}

	object* write(object* x, output_port* port, bool quoted)
	{
		port->write(stringify(x, quoted));
		return x;
	}

	void pdebug(object* x)
	{
		std::cout << stringify(x) << std::endl << std::flush;
	}

	void pdebug(const std::string& msg, object* x)
	{
		std::cout << msg << ": " << stringify(x) << std::endl << std::flush;
	}

	object* eval(object* x, environment* env)
	{
		//
		// The purpose of the while loop is to allow tail recursion.
		// The idea is that in a tail recursive position, we do "x = ..."
		// and loop, rather than doing "return eval(...)".
		//
		while (true)
		{
			if (is_symbol(x))	// VARIABLE
			{
				symbol* sym = dynamic_cast<symbol*>(x);
				return env->lookup(sym);
			}
			else if (!is_pair(x)) // CONSTANT
			{
				return x;
			}
			else
			{
				object* fn = car(x);
				object* args = cdr(x);

				if (is_symbol(fn, "quote")) // QUOTE
				{
					return car(args);
				}
				else if (is_symbol(fn, "begin")) // BEGIN
				{
					for (; !is_null(cdr(args)); args = cdr(args))
					{
						eval(car(args), env);
					}
					x = car(args);
				}
				else if (is_symbol(fn, "define")) // DEFINE
				{
					if (is_pair(car(args)))
					{
						object* lx = cons(symbol::get("lambda"),
										  cons(cdr(car(args)), cdr(args)));
						return env->define(car(car(args)), eval(lx, env));
					}
					else
					{
						return env->define(first(args), eval(second(args), env));
					}
				}
				else if (is_symbol(fn, "set!")) // SET!
				{
					return env->set(first(args), eval(second(args), env));
				}
				else if (is_symbol(fn, "if")) // IF
				{
					object* test = eval(first(args), env);
					x = truth(test) ? second(args) : third(args);
				}
#if 0
				else if (is_symbol(fn, "cond")) // COND
				{
					x = reduceCond(args, env);
				}
#endif
				else if (is_symbol(fn, "lambda")) // LAMBDA
				{
					return new closure(first(args), second(args), env);
				}
#if 0
				else if (is_symbol(fn, "macro"))
				{
					// FIXME
					// return Macro(new macro(car(args), cdr(args), env);
				}
#endif
				else
				{
					fn = eval(fn, env);
					if (is_closure(fn))
					{
						closure* f = dynamic_cast<closure*>(fn);
						x = f->body();
						pdebug(x);
						env = new environment(f->params(),
											  evlist(args, env),
											  f->env());
					}
					else
					{
						pdebug(fn);
						primitive* p = dynamic_cast<primitive*>(fn);
						return p->apply(evlist(args, env));
					}
				}
			}
		}
	}

	object* evlist(object* lst, environment* env)
	{
		if (is_null(lst))
		{
			return NIL();
		}
		else if (!is_pair(lst))
		{
			return error("illegal arg list: " + stringify(lst));
		}
		else
		{
			object* x = eval(car(lst), env);
			return cons(x, evlist(cdr(lst), env));
		}
	}
}
