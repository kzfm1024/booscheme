//
// booscheme.h
//

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

class empty
{
};

#include <iostream> // debugging purpose

class symbol
{
public:
    symbol(const char* s) : str(s) {}
    ~symbol() { std::cout << "~symbol " << str << std::endl; }

public:
    const std::string str;
};

class environment;

typedef boost::shared_ptr<std::pair<boost::any, boost::any> > Pair;
typedef boost::shared_ptr<empty>  Empty;
typedef boost::shared_ptr<int> Number; // FIXME
typedef boost::shared_ptr<std::string> String;
typedef boost::shared_ptr<char> Char; // FIXME
typedef boost::shared_ptr<symbol> Symbol;
typedef boost::shared_ptr<std::vector<boost::any> > Vector;
typedef boost::shared_ptr<environment> Environment;

Empty null();
std::string stringify(boost::any x, bool quoted);
std::string stringify(boost::any x);
