//
// boo.h
//

#pragma once
#ifndef _BOO_H
#define _BOO_H

#include "boolean.h"
#include "null.h"
#include "pair.h"
#include "symbol.h"

namespace boo
{
    boolean* TRUE();
    boolean* FALSE();
    boolean* BOOLEAN(bool x);
    null* NIL();

	object* cons(object* a, object* b);
    object* car(object* x);
	object* cdr(object* x);
	object* setcar(object* x, object* a);
	object* setcdr(object* x, object* b);

    bool is_null(object* x);
    bool is_pair(object* x);
}

#endif // _BOO_H
