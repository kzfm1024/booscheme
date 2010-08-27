//
// booscheme.cpp
//

#include <iostream>
#include <stdexcept>
#include "booscheme.h"
using namespace std;

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

Pair cons(boost::any a, boost::any b)
{
    return Pair(new std::pair<boost::any, boost::any>(a, b));
}

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

static void stringify(boost::any x, bool quoted, std::string& buf);
static void stringifyPair(Pair p, bool quoted, std::string& buf);

void stringify(boost::any x, bool quoted, std::string& buf)
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

void stringifyPair(Pair p, bool quoted, std::string& buf)
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

void test1()
{
    cout << "### test1 ###" << endl;

    Pair pp = cons(String(new string("foo")), 3);

    cout << *(boost::any_cast<String>(pp->first)) << endl;
    cout << boost::any_cast<int>(pp->second) << endl;

    if (pp->first.type() == typeid(String))
    {
        cout << "first is String" << endl;
    }

    if (pp->first.type() == typeid(int))
    {
        cout << "first is int" << endl;
    }

    if (pp->second.type() == typeid(String))
    {
        cout << "second is String" << endl;
    }

    if (pp->second.type() == typeid(int))
    {
        cout << "second is int" << endl;
    }
}

void test2()
{
    cout << "### test2 ###" << endl;

    boost::any x = Symbol(new symbol("bar"));
    cout << boost::any_cast<Symbol>(x)->str << endl;

    cout << "HERE #1" << endl;

    x = Symbol(new symbol("hoge"));
    cout << boost::any_cast<Symbol>(x)->str << endl;    

    cout << "HERE #2" << endl;
}

void test3()
{
    cout << "### test3 ###" << endl;

    boost::any foo = Symbol(new symbol("foo"));
    boost::any bar = Symbol(new symbol("bar"));
    boost::any nil = Empty(new empty);
    
    cout << stringify(cons(foo, bar)) << endl;
    cout << stringify(cons(foo, cons(bar, nil))) << endl;
}

int main()
{
    test1();
    test2();
    test3();
}
