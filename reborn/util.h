//
// util.h
//

#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include "boolean.h"

namespace booscheme
{
	boolean* TRUE();
	boolean* FALSE();
	boolean* BOOLEAN(bool x);

	object* cons(object* a, object* b);
	object* car(object* x);
	object* cdr(object* x);
	object* setcar(object* x, object* a);
	object* setcdr(object* x, object* b);
}

#endif // _UTIL_H
