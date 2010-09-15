//
// environment.cpp
//

#include "booscheme.h"

environment::environment(boost::any vars, boost::any vals, Environment parent)
    : vars(vars), vals(vals), parent(parent)
{
}

boost::any environment::lookup(Symbol sym)
{
    boost::any varList = vars;
    boost::any valList = vals;

/*
    while (!isEmpty(varList))
    {
        if (first(varList) == sym)
        {
            return first(valList);
        }
        else if (varList == sym)
        {
            return valList;
        }
        else
        {
            varList = rest(varList);
            valList = rest(valList);
        }
    }
*/

    if (parent) return parent->lookup(sym);
    else return error("Unbound variable: " + sym->name());
}

boost::any environment::define(boost::any var, boost::any val)
{
#if 0
    try
    {
        Symbol sym = boost::any_cast<Symbol>(var);
        env[sym->name()] = val;
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME
    }
#endif
}

boost::any environment::set(boost::any var, boost::any val)
{
#if 0
    try
    {
        Symbol sym = boost::any_cast<Symbol>(var);
        std::map<std::string, boost::any>::iterator iter = env.find(sym->name());
        if (iter != env.end())
        {
            (*iter).second = val;
        }
        else
        {
            // FIXME error
        }
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME FIXME error
    }
#endif
}

bool environment::numberArgsOK(boost::any vars, boost::any vals)
{
    return ((vars.type() == typeid(Empty) && vals.type() == typeid(Empty)) ||
            (vars.type() == typeid(String)) ||
            (vars.type() == typeid(Pair) && vals.type() == typeid(Pair) &&
             numberArgsOK(rest(vars), rest(vals))));
}
