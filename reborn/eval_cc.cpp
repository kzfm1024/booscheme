//
// eval_cc.cpp
//

#include "boo.h"
#include "boo_types.h"
#include "continuation.h"

namespace boo
{
    object* eval_cc(object* x, environment* env, continuation* cc)
    {
        //
        // The purpose of the while loop is to allow tail recursion.
        // The idea is that in a tail recursive position, we do "x = ..."
        // and loop, rather than doing "return eval(...)".
        //
        while (true)
        {
            if (is_symbol(x))   // VARIABLE
            {
                symbol* sym = dynamic_cast<symbol*>(x);
				return cc->apply(env->lookup(sym));
            }
            else if (!is_pair(x)) // CONSTANT
            {
				return cc->apply(x);
            }
            else
            {
                object* fn = car(x);
                object* args = cdr(x);

                if (is_symbol(fn, "quote")) // QUOTE
                {
                    return cc->apply(car(args));
                }
#if 0
                else if (is_symbol(fn, "begin")) // BEGIN
                {
                    for (; !is_null(cdr(args)); args = cdr(args))
                    {
                        eval(car(args), env);
                    }
                    x = car(args);
                }
#endif
                else if (is_symbol(fn, "define")) // DEFINE
                {
#if 0
                    if (is_pair(car(args)))
                    {
                        object* lx = cons(symbol::get("lambda"),
                                          cons(cdr(car(args)), cdr(args)));
                        return env->define(car(car(args)), eval(lx, env));
                    }
                    else
#endif
                    {
						continuation_define* cc2 = new continuation_define(first(args), env, cc);
						return eval_cc(second(args), env, cc2);
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
					continuation* cc2 = new continuation_procedure(args, env, cc);
					return eval_cc(fn, env, cc2);
                }
            }
        }
    }

	object* evlist_cc(object* lst, environment* env, continuation* cc)
	{
		if (is_null(lst))
		{
			return cc->apply(lst);
		}
		else
		{
			continuation* cc2 = new continuation_evlist(cdr(lst), env, cc);
			return eval_cc(car(lst), env, cc2);
		}
	}
}
