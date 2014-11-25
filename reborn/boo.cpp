//
// boo.cpp
//

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h> // for strtol()
#include <errno.h>  // for strtol() error handling
#include <limits.h> // for strtol() error handling
#include <assert.h>
#include "boo.h"
#include "boo_objects.h"

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

    object* list(object* x)
    {
        return cons(x, NIL());
    }

    object* list(object* x, object* y)
    {
        return cons(x, cons(y, NIL()));
    }

	object* eq(object* x, object* y)
	{
		return x == y ? TRUE() : FALSE();
	}

	object* eqv(object* x, object* y)
	{
		if (is_boolean(x) && is_boolean(y))
		{
			return truth(x) == truth(y) ? TRUE() : FALSE();
		}
		else if (is_symbol(x) && is_symbol(y))
		{
			return x == y ? TRUE() : FALSE();
		}
		else if (is_number(x) && is_number(y))
		{
			number* nx = dynamic_cast<number*>(x);
			number* ny = dynamic_cast<number*>(y);
			return nx->to_i() == ny->to_i() ? TRUE() : FALSE();
		}
		else if (is_null(x) && is_null(y))
		{
			return TRUE();
		}
		else if (is_pair(x) && is_pair(y))
		{
			return x == y ? TRUE() : FALSE();
		}
		else if (is_procedure(x) && is_procedure(y))
		{
			return x == y ? TRUE() : FALSE();
		}

		return FALSE();
	}

	object* equal(object* x, object* y)
	{
		// NOT YET
		return FALSE();
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

	bool is_boolean(object* x)
	{
        boolean* b = dynamic_cast<boolean*>(x);
		return b ? true : false;
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

	bool is_syntax(object* x)
	{
		syntax* synx = dynamic_cast<syntax*>(x);
		return synx ? true : false;
	}

    bool is_procedure(object* x)
    {
        procedure* proc = dynamic_cast<procedure*>(x);
        return proc ? true : false;
    }

    bool is_procedure(object* x, const std::string& s)
    {
        procedure* proc = dynamic_cast<procedure*>(x);
        return (proc && proc->name() == s) ? true : false;
    }

    bool is_closure(object* x)
    {
        closure* cl = dynamic_cast<closure*>(x);
        return cl ? true : false;
    }

	bool is_continuation(object* x)
	{
		continuation* cc = dynamic_cast<continuation*>(x);
		return cc ? true : false;
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
}
