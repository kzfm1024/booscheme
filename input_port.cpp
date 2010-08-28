//
// input_port.cpp
//

#include "booscheme.h"

std::string input_port::eof("#!EOF"); // FIXME

boost::any input_port::readChar()
{
    if (isPushedChar)
    {
        isPushedChar = false;
    }
}

    boost::any peekChar();
    int peekCh();
    boost::any read();

    int pushChar(int ch);
    int popChar();
