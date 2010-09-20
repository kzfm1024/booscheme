//
// clojure.cpp
//

#include "booscheme.h"

closure::closure(boost::any p, boost::any b, Environment e)
    : parms(p), env(e)
{
    if (isPair(b) && isEMPTY(rest(b)))
    {
        body = first(b);
    }
    else
    {
        body = cons(symbol::make("begin"), body);
    }
}

boost::any closure::apply(interpreter* interp, boost::any args)
{
    return interp->eval(body, Environment(new environment(parms, args, env)));
}
