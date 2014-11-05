//
// util.cpp
//

#include <assert.h>
#include "util.h"
#include "pair.h"

namespace booscheme
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
}
