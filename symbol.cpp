//
// symbol.cpp
//

#include "booscheme.h"

std::map<std::string, Symbol> symbol::table;

Symbol symbol::make(const std::string& s)
{
    return make(s, ORDINARY);    
}

#if 0
Symbol symbol::makeOrdinary(const std::string& s)
{
    return make(s, ORDINARY);
}

Symbol symbol::makeSyntax(const std::string& s)
{
    return make(s, SYNTAX);
}

Symbol symbol::makeInternal(const std::string& s)
{
    return make(s, INTERNAL);
}
#endif

Symbol symbol::make(const std::string& s, int kind)
{
    Symbol sym;
    std::map<std::string, Symbol>::iterator iter = table.find(s);
    if (iter != table.end())
    {
        sym = iter->second;
    }
    else
    {
        sym = Symbol(new symbol(s, kind));
        table[s] = sym;
    }
    
    return sym;
}
