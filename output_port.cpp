//
// output_port.cpp
//

#include "booscheme.h"

void output_port::write(const std::string& s)
{
    out << s;
}

void output_port::newline()
{
    out << std::endl;
}

void output_port::flush()
{
    out << std::flush;
}
