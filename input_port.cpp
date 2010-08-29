//
// input_port.cpp
//

#include "booscheme.h"

bool input_port::isEOF(boost::any x)
{
    return (x.type() == typeid(end_of_file)) ? true : false;
}

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
    std::string msg;

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
    case ';': // Comment: skip to end of line and then read next token
        while (!in.eof() && ch != '\n' && ch != '\r') in >> ch;
        return nextToken();
    case '"': // String
        buff.clear();
        in >> ch;
        while (ch != '"' && !in.eof())
        {
            if (ch == '\\') in >> ch;
            buff.append(1, ch);
            in >> ch;
        }
        if (in.eof()) warn("EOF inside of a string.");
        return String(new std::string(buff));
    case '#':
        in >> ch;
        switch (ch)
        {
        case 't': case 'T': return true;
        case 'f': case 'F': return false;
        case '(': // Vector
            pushChar('(');
            // return listToVector(read());
            return read(); // FIXME
        case '\\': // Char
            in >> ch;
            if (ch == 's' || ch == 'S' || ch == 'n' || ch == 'N')
            {
                pushChar(ch);
                boost::any token = nextToken();
                try
                {
                    Symbol sym = boost::any_cast<Symbol>(token);
                    if (!sym->str.compare("space")) return chr(' ');
                    if (!sym->str.compare("newline")) return chr('\n');
                }
                catch (const boost::bad_any_cast& e)
                {
                    ; // do nothing                    
                }

                isPushedToken = true;
                pushedToken = token;
                return chr(ch);
            }
            else
            {
                return chr(ch);
            }
        case 'e': case 'i': case 'd': 
        case 'b': case 'o': case 'x':
            msg.assign("#");
            msg.append(1, ch);
            msg.append(" not implemented, ignored.");
            warn(msg);
            return nextToken();
        default:
            msg.assign("#");
            msg.append(1, ch);
            msg.append(" not recognized, ignored.");
            return nextToken();
        }
    default:
        buff.clear();
        char c = ch;
        do
        {
            buff.append(1, ch);
            in >> ch;
        } while (!in.eof() &&
                 ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' &&
                 ch != '(' && ch != ')' && ch != '\'' && ch != ';' &&
                 ch != '"' && ch != ',' && ch != '`');
        pushChar(ch);
        //
        // FIXME 数字への変換を試みる
        // 
        return Symbol(new symbol(buff));
    }
}
