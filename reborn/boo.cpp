//
// boo.cpp
//

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <assert.h>
#include "boo.h"

#include "boolean.h"
#include "null.h"
#include "pair.h"
#include "number.h"
#include "character.h"
#include "symbol.h"
#include "misc.h"
#include "environment.h"
#include "procedure.h"

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

	object* UNDEF()
	{
		static misc* undef;
		if (!undef) undef = new misc("#<undef>");
		return dynamic_cast<object*>(undef);
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

	pair* cons(object* a, object* b)
	{
		return new pair(a, b);
	}

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
		return (sym && sym->to_s() == s) ? true : false;
	}

	bool is_procedure(object* x, const std::string& s)
	{
		procedure* proc = dynamic_cast<procedure*>(x);
		return (proc && proc->to_s() == s) ? true : false;
	}

	static void stringify_pair(pair* p, bool quoted, std::ostringstream& buf);

	static void stringify(object* x, bool quoted, std::ostringstream& buf)
	{
		if (is_pair(x))
		{
			stringify_pair(dynamic_cast<pair*>(x), quoted, buf);
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

	void print(object* x)
	{
		std::cout << stringify(x) << std::endl << std::flush;
	}

	void print(const std::string& msg, object* x)
	{
		std::cout << msg << ": " << stringify(x) << std::endl << std::flush;
	}
}
