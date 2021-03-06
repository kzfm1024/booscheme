//
// eval_cc.cpp
//

#include "boo.h"
#include "boo_objects.h"

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
				DEBUG("eval_cc sym", sym);
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
                else if (is_symbol(fn, "begin")) // BEGIN
                {
					continuation* cc2 = new continuation_begin(cdr(args), env, cc);
					return eval_cc(car(args), env, cc2);
                }
                else if (is_symbol(fn, "define")) // DEFINE
                {
                    if (is_pair(car(args)))
                    {
                        object* lx = cons(symbol::get("lambda"),
                                          cons(cdr(car(args)), cdr(args)));
						continuation_define* cc2 = new continuation_define(car(car(args)), env, cc);
						return eval_cc(lx, env, cc2);
					}
                    else
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
                    return cc->apply(new closure(first(args), second(args), env));
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
					DEBUG("#0 x", x);
					DEBUG("#1 fn", fn);
					DEBUG("#2 args", args);
					DEBUG("#3 cc", cc);
					continuation* cc2 = new continuation_procedure(args, env, cc);
					return eval_cc(fn, env, cc2);
                }
            }
        }
    }

	object* evlist_cc(object* lst, environment* env, continuation* cc)
	{
		DEBUG("evlist_cc lst", lst);
		DEBUG("evlist_cc cc", cc);

		if (is_null(lst))
		{
			return cc->apply(lst);
		}
        else if (!is_pair(lst))
        {
            return error("illegal arg list: " + stringify(lst));
        }
		else
		{
			continuation* cc2 = new continuation_evlist(cdr(lst), env, cc);
			return eval_cc(car(lst), env, cc2);
		}
	}
}
