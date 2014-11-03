//
// symbol.cpp
//

#include <sstream>
#include "symbol.h"

namespace booscheme
{
	std::string symbol::inspect()
	{
		std::ostringstream stream;
		stream << "#<symbol " << m_name << ":" << this << ">";
		return stream.str();
	}

	std::map<std::string, symbol*> symbol::m_symtab;

	symbol* symbol::get(const std::string& name)
	{
		symbol* sym;
		std::map<std::string, symbol*>::iterator iter = m_symtab.find(name);
		if (iter != m_symtab.end())
		{
			sym = iter->second;
		}
		else
		{
			sym = new symbol(name);
			m_symtab[name] = sym;
		}
    
		return sym;
	}
}
