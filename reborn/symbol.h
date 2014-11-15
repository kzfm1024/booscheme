//
// symbol.h
//

#pragma once
#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <map>
#include "object.h"

namespace boo
{
    class symbol : public object
    {
    private:
        symbol(const std::string& name) : m_name(name) {}
        symbol(const symbol& sym) : m_name(sym.m_name) {}
        virtual ~symbol() {} 
        symbol& operator=(const symbol& sym) { m_name = sym.m_name; }

    public:
        virtual std::string to_s() { return m_name; }
        virtual std::string inspect();

        static symbol* get(const std::string& name);

    private:
        static std::map<std::string, symbol*> m_symtab;
        std::string m_name;
    };
}

#endif // _SYMBOL_H
