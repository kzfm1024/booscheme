//
// environment.cpp
//

#include "booscheme.h"

environment::environment()
    : vars(EMPTY()), vals(EMPTY()), parent()
{
}

environment::environment(boost::any vars, boost::any vals, Environment parent)
    : vars(vars), vals(vals), parent(parent)
{
}

boost::any environment::lookup(Symbol s)
{
    boost::any varList = vars;
    boost::any valList = vals;

    while (!isEMPTY(varList))
    {
        if (sym(first(varList)) == s)
        {
            return first(valList);
        }
        else if (sym(varList) == s)
        {
            return valList;
        }
        else
        {
            varList = rest(varList);
            valList = rest(valList);
        }
    }

    if (parent) return parent->lookup(s);
    else return error("unbound variable: " + s->name);
}

boost::any environment::define(boost::any var, boost::any val)
{
    vars = cons(var, vars);
    vals = cons(val, vals);

    if (isProcedure(val, "anonymous procedure"))
    {
        proc(val)->name = sym(var)->name;
    }

    return var;
}

boost::any environment::set(boost::any var, boost::any val)
{
    if (!isSymbol(var))
    {
        return error("attempt to set a non-symbol: " + stringify(var));
    }

    Symbol s = sym(var);
    boost::any varList = vars;
    boost::any valList = vals;

    while (!isEMPTY(varList))
    {
        if (sym(first(varList)) == s)
        {
            return setFirst(valList, val);
        }
        else if (sym(rest(varList)) == s)
        {
            return setRest(valList, val);
        }
        else
        {
            varList = rest(varList);
            valList = rest(valList);
        }
    }

    if (parent) return parent->set(s, val);
    else return error("unbound variable: " + s->name);
}

bool environment::numberArgsOK(boost::any vars, boost::any vals)
{
    return ((isEMPTY(vars) && isEMPTY(vals)) ||
            isSymbol(vars) ||
            (isPair(vars) && isPair(vals) &&
             numberArgsOK(rest(vars), rest(vals))));
}
