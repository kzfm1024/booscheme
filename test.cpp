//
// test.cpp
//

#include <iostream>
// #include <stdexcept>
#include "booscheme.h"
using namespace std;

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

    bool b;
    boost::any a = b;

    if (a.type() == typeid(bool))
    {
        cout << "a is bool." << endl;
    }
}
