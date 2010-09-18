//
// interpreter.cpp
//

#include "booscheme.h"

interpreter::interpreter()
{
    input = InputPort(new input_port(std::cin));
    output = OutputPort(new output_port(std::cout));
    globalEnvironment = Environment(new environment);
}

void interpreter::repl()
{
    boost::any x;

    while (true)
    {
        try
        {
            output->write("boo> "); output->flush();
            if (isEOF(x = input->read())) return;
            write(eval(x), output, true);
            output->newline();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

boost::any interpreter::eval(boost::any x, Environment env)
{
    //
    // The purpose of the while loop is to allow tail recursion.
    // The idea is that in a tail recursive position, we do "x = ..."
    // and loop, rather than doing "return eval(...)".
    //
    while (true)
    {
        if (isSymbol(x))        // VARIABLE
        {
            return env->lookup(sym(x));
        }
        else if (!isPair(x))    // CONSTANT
        {
            return x;
        }
        else
        {
            boost::any fn = first(x);
            boost::any args = rest(x);
            if (isSymbol(fn, "quote")) // QUOTE
            {
                return first(args);
            }
            else if (isSymbol(fn, "begin")) // BEGIN
            {
                for (; !isEmpty(rest(args)); args = rest(args))
                {
                    eval(first(args), env);
                }
                x = first(args);
            }
            else if (isSymbol(fn, "define")) // DEFINE
            {
                if (isPair(first(args)))
                {
                    return env->define(first(first(args)),
                                       eval(cons(symbol::make("lambda"),
                                                 cons(rest(first(args)), rest(args))), env));
                }
                else
                {
                    return env->define(first(args), eval(second(args), env));
                }
            }
            else if (isSymbol(fn, "set!")) // SET!
            {
                return env->set(first(args), eval(second(args), env));
            }
            else if (isSymbol(fn, "if")) // IF
            {
                x = (truth(eval(first(args), env))) ? second(args) : third(args);
            }
            else if (isSymbol(fn, "cond")) // COND
            {
                x = reduceCond(args, env);
            }
            else if (isSymbol(fn, "lambda")) // LAMBDA
            {
                return Closure(new closure(first(args), rest(args), env));
            }
            else if (isSymbol(fn, "macro"))
            {
                // return Macro(new macro(first(args), rest(args), env); NOT YET
            }
            else
            {
                fn = eval(fn, env);
                if (isMacro(fn))
                {
                    // FIXME
                }
                else if (isClosure(fn))
                {
                    Closure f = boost::any_cast<Closure>(fn);
                    x = f->body;
                    env = Environment(new environment(f->parms, evalList(args, env), f->env));
                }
                else
                {
                    return x; // NOT YET: apply
                }
            }
        }
    }
}

boost::any interpreter::eval(boost::any x)
{
    return eval(x, globalEnvironment);
}

boost::any interpreter::evalList(boost::any list, Environment env)
{
    if (isEmpty(list))
    {
        return empty();
    }
    else if (!isPair(list))
    {
        return error("illegal arg list: " + stringify(list));
    }
    else
    {
        boost::any x = eval(first(list), env);
        return cons(x, evalList(rest(list), env));
    }
}

/** Reduce a cond expression to some code which, when evaluated,
 * gives the value of the cond expression.  We do it that way to
 * maintain tail recursion. **/
boost::any interpreter::reduceCond(boost::any clauses, Environment env)
{
    boost::any result;
    
    while (true)
    {
        if (isEmpty(clauses)) return false; // FIXME

        boost::any clause = first(clauses);
        clauses = first(clauses);

        if (isSymbol(first(clause), "else") || truth(result = eval(first(clause), env)))
        {
            if (isEmpty(rest(clause)))
            {
                return list(symbol::make("quote"), result);
            }
            else if (isSymbol(second(clause), "=>"))
            {
                return list(third(clause), list(symbol::make("quote"), result));
            }
            else
            {
                return cons(symbol::make("begin"), rest(clause));
            }
        }
    }
}
