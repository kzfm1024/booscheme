//
// utils.cpp
//

#include <iostream>
#include <stdexcept>
#include "booscheme.h"

void error(const std::string& message)
{
    throw std::runtime_error("*** ERROR: " + message);
}

void warn(const std::string& message)
{
    std::cerr << message << std::endl;
}

Empty null()
{
    return Empty(new empty);
}

bool truth(boost::any x)
{
    if (x.type() == typeid(bool) && boost::any_cast<bool>(x) == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int num(boost::any x)
{
    try
    {
        return boost::any_cast<int>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME
    }
}

char chr(boost::any x)
{
    try
    {
        return boost::any_cast<char>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME
    }
}

String str(boost::any x)
{
    try
    {
        return boost::any_cast<String>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        // FIXME
    }
}

//
// NOT YET
// vec()
// inPort()
// outPort()
//

boost::any first(boost::any x)
{
    try
    {
        return boost::any_cast<Pair>(x)->first;
    }
    catch (const boost::bad_any_cast& e)
    {
        return null();
    }
}    

boost::any rest(boost::any x)
{
    try
    {
        return boost::any_cast<Pair>(x)->second;
    }
    catch (const boost::bad_any_cast& e)
    {
        return null();
    }
}

boost::any setFirst(boost::any x, boost::any y)
{
    try
    {
        return boost::any_cast<Pair>(x)->first = y;
    }
    catch (const boost::bad_any_cast& e)
    {
        // error FIXME
    }
}

boost::any setRest(boost::any x, boost::any y)
{
    try
    {
        return boost::any_cast<Pair>(x)->second = y;
    }
    catch (const boost::bad_any_cast& e)
    {
        // error FIXME
    }
}

boost::any second(boost::any x)
{
    return first(rest(x));
}

boost::any third(boost::any x)
{
    return first(rest(rest(x)));
}

boost::any list(boost::any a, boost::any b)
{
    return cons(a, cons(b, Empty(new empty)));
}

boost::any list(boost::any a)
{
    return cons(a, null());
}

boost::any listStar(boost::any args)
{
    if (rest(args).type() == typeid(Empty)) return first(args);
    else return cons(first(args), listStar(rest(args)));
}

Pair cons(boost::any a, boost::any b)
{
    return Pair(new std::pair<boost::any, boost::any>(a, b));
}

boost::any reverse(boost::any x)
{
    boost::any result = null();
    while (x.type() == typeid(Pair))
    {
        result = cons(first(x), result);
        x = rest(x);
    }
    return result;
}

// equal()
// eqv()

int length(boost::any x)
{
    int len = 0;
    while (x.type() == typeid(Pair))
    {
        len++;
        x = rest(x);
    }
    return len;
}

boost::any listToString(boost::any chars)
{
    String str = String(new std::string());
    for (int i = 0; chars.type() == typeid(Pair); i++)
    {
        str->append(1, chr(first(chars)));
        chars = rest(chars);
    }

    return str;
}

boost::any listToVector(boost::any objs)
{
    Vector vec = Vector(new std::vector<boost::any>(length(objs)));
    for (int i = 0; objs.type() == typeid(Pair); i++)
    {
        vec->at(i) = first(objs);
        objs = rest(objs);
    }

    return vec;
}

boost::any vectorToList(boost::any x)
{
    if (x.type() == typeid(Vector))
    {
        Vector vec = boost::any_cast<Vector>(x);
        boost::any result = null();
        for (int i = vec->size() - 1; i >= 0; i--)
        {
            result = cons(vec->at(i), result);
        }
        return result;
    }
    else
    {
        // NOT YET: error
        return null();
    }
}

boost::any write(boost::any x, OutputPort port, bool quoted)
{
    port->write(stringify(x, quoted));
    return x;
}

static void stringifyPair(Pair p, bool quoted, std::string& buf);

static void stringify(boost::any x, bool quoted, std::string& buf)
{
    if (x.type() == typeid(Empty))
    {
        buf.append("()");
    }
    else if (x.type() == typeid(Number))
    {
        // NOT YET
    }
    else if (x.type() == typeid(Char))
    {
        if (quoted) buf.append("#\\");
        buf.append(std::string(1, *(boost::any_cast<Char>(x))));
    }
    else if (x.type() == typeid(Pair))
    {
        stringifyPair(boost::any_cast<Pair>(x), quoted, buf);
    }
    else if (x.type() == typeid(String))
    {
        buf.append(*(boost::any_cast<String>(x)));
    }
    else if (x.type() == typeid(Vector))
    {
        // NOT YET
    }
    else if (x.type() == typeid(bool))
    {
        if (boost::any_cast<bool>(x))
        {
            buf.append("#t");
        }
        else
        {
            buf.append("#f");
        }
    }
    else if (x.type() == typeid(Symbol))
    {
        buf.append(boost::any_cast<Symbol>(x)->str);
    }
    else
    {
        // error
    }
}

static void stringifyPair(Pair p, bool quoted, std::string& buf)
{
    std::string special;

    if ((p->second.type() == typeid(Pair)) &&
        rest(p->second).type() == typeid(Empty))
    {
        try
        {
            Symbol sym = boost::any_cast<Symbol>(p->first);
            if      (!sym->str.compare("quote"))            { special = "'";  }
            else if (!sym->str.compare("quasiquote"))       { special = "`";  }
            else if (!sym->str.compare("unquote"))          { special = ",";  }
            else if (!sym->str.compare("unquote-splicing")) { special = ",@"; }
        }
        catch (const boost::bad_any_cast& e)
        {
            ; // do nothing
        }   
    }

    if (special.size())
    {
        buf.append(special);
        stringify(second(p), quoted, buf);
    }
    else
    {
        buf.append("(");
        stringify(p->first, quoted, buf);

        boost::any tail = p->second;
        while (tail.type() == typeid(Pair))
        {
            buf.append(" ");
            stringify(boost::any_cast<Pair>(tail)->first, quoted, buf);
            tail = boost::any_cast<Pair>(tail)->second;
        }
        
        if (tail.type() != typeid(Empty))
        {
            buf.append(" . ");
            stringify(tail, quoted, buf);
        }

        buf.append(")");
    }
}

std::string stringify(boost::any x, bool quoted)
{
    std::string str;
    stringify(x, quoted, str);
    return str;
}

std::string stringify(boost::any x)
{
    return stringify(x, true);
}
        
boost::any p(boost::any x)
{
    std::cout << stringify(x) << std::endl << std::flush;
}

boost::any p(const std::string& msg, boost::any x)
{
    std::cout << msg << ": " << stringify(x) << std::endl << std::flush;
}

bool isEOF(boost::any x)
{
    return (x.type() == typeid(end_of_file)) ? true : false;
}

bool isSymbol(boost::any x, const char* s)
{
    try
    {
        Symbol sym = boost::any_cast<Symbol>(x);
        return !sym->str.compare(s) ? true : false;
    }
    catch (const boost::bad_any_cast& e)
    {
        return false;
    }
}

bool isMisc(boost::any x, const char* s)
{
    try
    {
        Misc m = boost::any_cast<Misc>(x);
        return !m->str.compare(s) ? true : false;
    }
    catch (const boost::bad_any_cast& e)
    {
        return false;
    }
}
