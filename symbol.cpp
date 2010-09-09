//
// symbol.cpp
//

#include "booscheme.h"

std::map<std::string, Symbol> symbol::table;

Symbol symbol::make(const std::string& s)
{
    Symbol sym;
    std::map<std::string, Symbol>::iterator iter = table.find(s);
    if (iter != table.end())
    {
        sym = iter->second;
    }
    else
    {
        sym = Symbol(new symbol(s));
        table[s] = sym;
    }
    
    return sym;
}
