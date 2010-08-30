//
// output_port.cpp
//

#include "booscheme.h"

void output_port::write(const std::string& s)
{
    out << s << std::flush;
}
