//
// test.cpp
//

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

    boost::any x = symbol::make("bar");
    cout << boost::any_cast<Symbol>(x)->name << endl;

    cout << "HERE #1" << endl;

    x = symbol::make("hoge");
    cout << boost::any_cast<Symbol>(x)->name << endl;
    
    cout << "HERE #2" << endl;
}

void test3()
{
    cout << "### test3 ###" << endl;

    boost::any foo = symbol::make("foo");
    boost::any bar = symbol::make("bar");
    // boost::any nil = Empty(new empty);
    
    cout << stringify(cons(foo, bar)) << endl;
    cout << stringify(cons(foo, cons(bar, empty()))) << endl;
}

void test4()
{
    cout << "### test4 ###" << endl;

    bool b;
    boost::any a = b;

    if (a.type() == typeid(bool))
    {
        cout << "a is bool." << endl;
    }
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    
    interpreter interp;
    interp.repl();
}
