//
// utils.cpp
//

#include <stdio.h> // for strtol()
#include <limits.h>
#include <errno.h>

#include <iostream>
#include <sstream>
#include "booscheme.h"

Misc error(const std::string& message)
{
    throw std::runtime_error("*** ERROR: " + message);
    return Misc(new misc("<error>"));
}

Misc warn(const std::string& message)
{
    std::cerr << "*** WARNING: " << message << std::endl;
    return Misc(new misc("<warn>"));
}

Empty null()
{
    return Empty(new empty); // FIXME
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

#if 0
long int num(boost::any x)
{
    try
    {
        return boost::any_cast<long int>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        error("expected a number, got: " + stringify(x));
        return 0;
    }
}
#endif

char chr(boost::any x)
{
    try
    {
        return boost::any_cast<char>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        error("expected a char, got: " + stringify(x));
        return 0;
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
        error("expected a string, got: " + stringify(x));
        return String(new std::string);
    }
}

Vector vec(boost::any x)
{
    try
    {
        return boost::any_cast<Vector>(x);
    }
    catch (const boost::bad_any_cast& e)
    {
        error("expected a vector, got: " + stringify(x));
        return Vector(new std::vector<boost::any>);
    }
}

//
// NOT YET
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
        return error("attempt to set-car of a non-Pair: " + stringify(x));
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
        return error("attempt to set-cdr of a non-Pair: " + stringify(x));
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
    return cons(a, cons(b, null()));
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
        error("expected a vector, got: " + stringify(x));
        return null();
    }
}

boost::any write(boost::any x, OutputPort port, bool quoted)
{
    port->write(stringify(x, quoted));
    return x;
}

static void stringifyPair(Pair p, bool quoted, std::ostringstream& buf);

static void stringify(boost::any x, bool quoted, std::ostringstream& buf)
{
    if (x.type() == typeid(Empty))
    {
        buf << "()";     
    }
    else if (x.type() == typeid(Number))
    {
        buf << *(boost::any_cast<Number>(x));
    }
    else if (x.type() == typeid(Char))
    {
        if (quoted) buf << "#\\"; // buf.append("#\\");
        buf << std::string(1, *(boost::any_cast<Char>(x)));
    }
    else if (x.type() == typeid(Pair))
    {
        stringifyPair(boost::any_cast<Pair>(x), quoted, buf);
    }
    else if (x.type() == typeid(String))
    {
        buf << *(boost::any_cast<String>(x));
    }
    else if (x.type() == typeid(Vector))
    {
        buf << vec(x); // FIXME
    }
    else if (x.type() == typeid(bool))
    {
        if (boost::any_cast<bool>(x))
        {
            buf << "#t";
        }
        else
        {
            buf << "#f";
        }
    }
    else if (x.type() == typeid(Symbol))
    {
        buf << boost::any_cast<Symbol>(x)->name();
    }
    else if (x.type() == typeid(Misc))
    {
        buf << boost::any_cast<Misc>(x)->str;
    }
    else 
    {
        error("attempt to stringify unknown object");
    }
}

static void stringifyPair(Pair p, bool quoted, std::ostringstream& buf)
{
    std::string special;

    if ((p->second.type() == typeid(Pair)) &&
        rest(p->second).type() == typeid(Empty))
    {
        if      (isSymbol(p->first, "quote"))            { special = "'";  }
        else if (isSymbol(p->first, "quasiquote"))       { special = "`";  }
        else if (isSymbol(p->first, "unquote"))          { special = ",";  }
        else if (isSymbol(p->first, "unquote-splicing")) { special = ",@"; }
    }

    if (special.size())
    {
        buf << special;
        stringify(second(p), quoted, buf);
    }
    else
    {
        buf << "(";
        stringify(p->first, quoted, buf);

        boost::any tail = p->second;
        while (tail.type() == typeid(Pair))
        {
            buf << " ";
            stringify(boost::any_cast<Pair>(tail)->first, quoted, buf);
            tail = boost::any_cast<Pair>(tail)->second;
        }
        
        if (tail.type() != typeid(Empty))
        {
            buf << " . ";
            stringify(tail, quoted, buf);
        }

        buf << ")";
    }
}

std::string stringify(boost::any x, bool quoted)
{
    std::ostringstream buf;
    stringify(x, quoted, buf);
    return buf.str();
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
        return !sym->name().compare(s) ? true : false;
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

Number toNumber(const std::string& s)
{
    long int val;
    char* endptr;
    int base = 10;
            
    val = strtol(s.c_str(), &endptr, base);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)))
    {
        throw std::invalid_argument("out of ranage");
    }
            
    if (*endptr)
    {
        throw std::invalid_argument("not a number");
    }

    return Number(new long int(val));
}
