//
// clojure.cpp
//

#include "booscheme.h"

closure::closure(boost::any p, boost::any b, Environment e)
    : params(p), env(e)
{
    if (b.type() == typeid(Pair) && rest(b).type() == typeid(Empty))
    {
        body = first(b);
    }
    else
    {
        body = cons(symbol::make("begin"), body);
    }
}
