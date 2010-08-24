//
// environment.h
//

#include "booscheme.h"

class environment
{
public:
    environment() : env(), parent() {}
    environment(boost::any vars, boost::any vals, Environment parent);
    ~environment() {}

    boost::any lookup(Symbol sym);
    boost::any define(boost::any var, boost::any val);
    boost::any set(boost::any var, boost::any val);

private:
    std::map<std::string, boost::any> env;
    Environment parent;
};
