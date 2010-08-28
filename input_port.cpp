//
// input_port.cpp
//

#include "booscheme.h"

std::string input_port::eof("#!EOF"); // FIXME

// Read and return a Scheme character or EOF.
boost::any input_port::readChar()
{
    if (isPushedChar)
    {
        isPushedChar = false;
        if (pushedChar == -1) { return eof;             }
        else                  { return chr(pushedChar); }
    }
    else
    {
        char ch;
        in >> ch;
        if      (in.bad()) { error("IO stream corrupted"); }
        else if (in.eof()) { return eof;                   }
        else               { return chr(ch);               }
    }
}

boost::any input_port::peekChar()
{
    int p = peekCh();
    if (p == -1) { return eof;    }
    else         { return chr(p); }
}

int input_port::peekCh()
{
    if (isPushedChar)
    {
        return pushedChar;
    }
    else
    {
        char ch;
        in >> ch;
        if (in.bad()) error("IO stream corrupted");
        return pushChar(in.eof() ? -1 : ch);
    }
}

int input_port::pushChar(int ch)
{
    isPushedChar = true;
    return pushedChar = ch;
}

int input_port::popChar()
{
    isPushedChar = false;
    return pushedChar;
}

boost::any input_port::read()
{
    


}

boost::any input_port::readTail()
{

}

boost::any input_port::nextToken()
{
    char ch;

    if (isPushedToken)
    {
        isPushedToken = false;
        return pushedToken;
    }
    else if (isPushedChar)
    {
        ch = static_cast<int>(popChar()); // FIXME
    }
    else
    {
        in >> ch;
    }
    
    // Skip whitespace
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') in >> ch;

    if (in.eof()) return eof;

    switch (ch)
    {
    case '(' : return Symbol(new symbol("("));
    case ')' : return Symbol(new symbol("("));
    case '\'': return Symbol(new symbol("'"));
    case '`' : return Symbol(new symbol("`"));
    case ',':
        in >> ch;
        if (ch == '@') return Symbol(new symbol(",@"));
        else { pushChar(static_cast<int>(ch)); return Symbol(new symbol(",")); }
    case ';':
        while (!in.eof() && ch != '\n' && ch != '\r') in >> ch;
        return nextToken();
    case '"': // String
        ; // NOT YET
    }
}
