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
            std::cout << e.what() << std::endl; // FIXME
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
        if (isSymbol(x))
        {
            return env->lookup(sym(x));
        }
        else if (!isPair(x))
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
                // NOT YET
            }
            else if (isSymbol(fn, "set!")) // SET!
            {
                // NOT YET
            }
            else if (isSymbol(fn, "if")) // IF
            {

            }
            else if (isSymbol(fn, "cond")) // COND
            {

            }
            else if (isSymbol(fn, "lambda")) // LAMBDA
            {

            }
            else if (isSymbol(fn, "macro"))
            {

            }
            else
            {
                fn = eval(fn, env);
                if (isMacro(fn))
                {

                }
                else if (isClosure(fn))
                {

                }
                else
                {
                    return x; // NOT YET
                }
            }
        }
    }
}

boost::any interpreter::eval(boost::any x)
{
    return eval(x, globalEnvironment);
}

//
// evalList()
// reduceCond()
//
