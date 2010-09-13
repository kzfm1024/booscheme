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

    while (1)
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
    return x; // NOT YET
}

boost::any interpreter::eval(boost::any x)
{
    return eval(x, globalEnvironment);
}
