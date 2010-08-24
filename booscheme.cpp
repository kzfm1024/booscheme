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

int main()
{
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

#if 0
    Symbol sym = Symbol(new symbol("bar"));
    cout << sym->str << endl;    

    sym = Symbol(new symbol("hoge"));
    cout << sym->str << endl;    
#else
    boost::any x = Symbol(new symbol("bar"));
    cout << boost::any_cast<Symbol>(x)->str << endl;

    x = Symbol(new symbol("hoge"));
    cout << boost::any_cast<Symbol>(x)->str << endl;    
#endif
}
