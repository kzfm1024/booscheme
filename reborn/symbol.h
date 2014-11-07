//
// symbol.h
//

#pragma once
#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <string>
#include <map>
#include "object.h"

namespace boo
{
	class symbol : public object
	{
	public:
		symbol(const std::string& name) : m_name(name) {}
		virtual ~symbol() {} 

		virtual std::string to_s() { return m_name; }
		virtual std::string inspect();

	public:
		static symbol* get(const std::string& name);

	private:
		static std::map<std::string, symbol*> m_symtab;
		std::string m_name;
	};
}

#endif // _SYMBOL_H
