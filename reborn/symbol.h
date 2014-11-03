//
// symbol.h
//

#pragma once
#ifndef _SYMBOL_H
#define _SYMBOL_H

class symbol
{
public:
    symbol(const std::string& n) : name(n) {}
    ~symbol() {} 

    static Symbol make(const std::string& n); // FIXME: change to get()?
    std::string name;

private:
    static std::map<std::string, Symbol> table;
};

#endif // _SYMBOL_H
