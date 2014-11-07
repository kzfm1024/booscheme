//
// boo.cpp
//

#include <assert.h>
#include "boo.h"


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

	object* cons(object* a, object* b)
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
}
