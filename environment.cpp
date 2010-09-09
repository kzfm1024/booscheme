//
// environment.cpp
//

#include "booscheme.h"

environment::environment(boost::any vars, boost::any vals, Environment parent)
    : env(), parent(parent)
{
    // NOT YET
}

boost::any environment::lookup(Symbol sym)
{
    std::map<std::string, boost::any>::iterator iter = env.find(sym->name());
    if (iter != env.end())
    {
        return *iter;
    }
    else if (parent.get())
    {
        return parent->lookup(sym);
    }
    else
    {
        // FIXME
        // "Unbound variable: " + sym->str
        return null();
    }
}

boost::any environment::define(boost::any var, boost::any val)
{
    try
    {
        Symbol sym = boost::any_cast<Symbol>(var);
        env[sym->name()] = val;
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME
    }
}

boost::any environment::set(boost::any var, boost::any val)
{
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
}

bool environment::numberArgsOK(boost::any vars, boost::any vals)
{
    return ((vars.type() == typeid(Empty) && vals.type() == typeid(Empty)) ||
            (vars.type() == typeid(String)) ||
            (vars.type() == typeid(Pair) && vals.type() == typeid(Pair) &&
             numberArgsOK(rest(vars), rest(vals))));
}
