//
// eval.cpp
//

#include <assert.h>
#include "boo.h"
#include "boo_objects.h"

namespace boo
{
    object* eval(object* x, environment* env)
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
				else if (is_symbol(fn, "define-syntax"))
				{
					symbol* sym = dynamic_cast<symbol*>(first(args));
					assert(sym);
					object* rule = second(args);
					syntax* synx = new syntax(sym->name(), rule);
					PDEBUG("define-syntax sym", sym);
					PDEBUG("define-syntax rule", rule);
#if 1 // TENTATIVE
					env->define(sym, synx);
					return synx;
#else
					return env->define(sym, synx);
#endif
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
                        env = new environment(f->params(),
                                              evlist(args, env),
                                              f->env());
                    }
                    else
                    {
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
