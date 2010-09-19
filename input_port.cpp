//
// input_port.cpp
//

#include <cctype>
#include "booscheme.h"

end_of_file input_port::eof;

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
        in.get(ch);
        if      (in.bad()) { return error("IO stream corrupted"); }
        else if (in.eof()) { return eof;                          }
        else               { return chr(ch);                      }
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
        in.get(ch);
        if (in.bad()) { error("IO stream corrupted"); return -1; }
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
    boost::any token = nextToken();

    if (isSymbol(token, "("))
    {
        return readTail();
    }
    else if (isSymbol(token, ")"))
    {
        warn("Extra ) ignored.");
        return read();
    }
    else if (isSymbol(token, "."))
    {
        warn("Extra . ignored.");
        return read();
    }
    else if (isSymbol(token, "'"))
    {
        return list(symbol::make("quote"), read());
    }
    else if (isSymbol(token, "`"))
    {
        return list(symbol::make("quasiquote"), read());
    }
    else if (isSymbol(token, ","))
    {
        return list(symbol::make("unquote"), read());
    }
    else if (isSymbol(token, ",@"))
    {
        return list(symbol::make("unquote-splicing"), read());
    }
    else
    {
        return token;
    }
}

boost::any input_port::readTail()
{
    boost::any token = nextToken();
    if (token.type() == typeid(end_of_file))
    {
        error("EOF during read.");
        return eof;
    }
    else if (isSymbol(token, ")"))
    {
        return EMPTY();
    }
    else if (isSymbol(token, "."))
    {
        boost::any result = read();
        token = nextToken();
        if (!isSymbol(token, ")")) 
        {
            warn("Where's the ')'? Got " + stringify(token) + " after ." );
        }
        return result;
    }
    else
    {
        isPushedToken = true;
        pushedToken = token;

        //
        // return cons(read(), readTail());
        //
        // 引数の評価順は仕様では不定である。
        // Intel x86 でよく使われる cdecl 規約では後の引数からスタックに
        // 積まれるため readTail() が無限ループしてしまう。
        // 
        boost::any x = read();
        return cons(x, readTail());
    }
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
        ch = static_cast<char>(popChar());
    }
    else
    {
        in.get(ch);
    }
    
    // Skip whitespace
    while (isspace(ch)) in.get(ch);

    if (in.bad()) return error("IO stream corrupted");
    if (in.eof()) return eof;
    
    switch (ch)
    {
    case '(' : return symbol::make("(");
    case ')' : return symbol::make(")");
    case '\'': return symbol::make("'");
    case '`' : return symbol::make("`");
    case ',':
        in.get(ch);
        if (ch == '@') return symbol::make(",@");
        else { pushChar(static_cast<int>(ch)); return symbol::make(","); }
    case ';': // Comment: skip to end of line and then read next token
        while (!in.eof() && ch != '\n' && ch != '\r') in.get(ch);
        return nextToken();
    case '"': // String
        buff.clear();
        in.get(ch);
        while (ch != '"' && !in.eof())
        {
            if (ch == '\\') in.get(ch);
            buff.append(1, ch);
            in.get(ch);
        }
        if (in.eof()) warn("EOF inside of a string.");
        return String(new std::string(buff));
    case '#':
        in.get(ch);
        switch (ch)
        {
        case 't': case 'T': return TRUE();
        case 'f': case 'F': return FALSE();
        case '(': // Vector
            pushChar('(');
            return listToVector(read());
        case '\\': // Char
            in.get(ch);
            if (ch == 's' || ch == 'S' || ch == 'n' || ch == 'N')
            {
                pushChar(ch);
                boost::any token = nextToken();
                if (isSymbol(token, "space"))
                {
                    return chr(' ');
                }
                else if (isSymbol(token, "newline"))
                {
                    return chr('\n');
                }
                else
                {
                    isPushedToken = true;
                    pushedToken = token;
                    return chr(ch);
                }
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
            in.get(ch);
        } while (!in.eof() && !isspace(ch) &&
                 ch != '(' && ch != ')' && ch != '\'' && ch != ';' &&
                 ch != '"' && ch != ',' && ch != '`');
        pushChar(ch);

        // Try potential numbers, but catch any format errors.
        if (c == '.' || c == '+' || c == '-' || (c >= '0' && c <= '9'))
        {
            try
            {
                return toNumber(buff); // FIXME
            }
            catch (const std::invalid_argument& e)
            {
                ; // do nothing
            }
        }

        return symbol::make(buff);
    }
}
