//
// primitive.cpp
//

#include "booscheme.h"

primitive::primitive(int id, int minArgs, int maxArgs)
    : id(id), minArgs(minArgs), maxArgs(maxArgs)
{
}

Environment
primitive::installPrimitives(Environment env)
{
    return env; // FIXME
}

boost::any primitive::apply(interpreter* interp, boost::any args)
{
    return TRUE(); // FIXME
}
