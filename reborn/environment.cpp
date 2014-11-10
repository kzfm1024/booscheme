//
// environment.cpp
//

#include <assert.h>
#include "environment.h"
#include "symbol.h"
#include "procedure.h"
#include "misc.h"
#include "boo.h"

namespace boo
{
	environment::environment()
		: m_parent(0), m_map()
	{
	}

	environment::environment(object* vars, object* vals, environment* parent)
		: m_parent(parent), m_map()
	{
		while (!is_null(vars))
		{
			symbol* var = dynamic_cast<symbol*>(car(vars));
			object* val = car(vals);
			
			assert(var);
			m_map[var] = val;
		
			vars = cdr(vars);
			vals = cdr(vals);
		}
	}

	object* environment::lookup(symbol* sym)
	{
		std::map<symbol*, object*>::iterator iter = m_map.find(sym);
		if (iter != m_map.end())
		{
			return (*iter).second;
		}
		else if (m_parent)
		{
			return m_parent->lookup(sym);
		}
		else
		{
			return error("unbound variable: " + stringify(sym));
		}
	}	

	object* environment::define(object* var, object* val)
	{
		symbol* sym = dynamic_cast<symbol*>(var);
		if (!sym)
		{
			return error("attempt to set a non-symbol: " + stringify(var));
		}

		if (is_procedure(val, "anonymous procedure"))
		{
			procedure* proc = dynamic_cast<procedure*>(val);
			proc->set_name(sym->to_s());
		}

		m_map[sym] = val;
		return sym;
	}

	object* environment::set(object* var, object* val)
	{
		symbol* sym = dynamic_cast<symbol*>(var);
		if (!sym)
		{
			return error("attempt to set a non-symbol: " + stringify(var));
		}
		
		std::map<symbol*, object*>::iterator iter = m_map.find(sym);
		if (iter != m_map.end())
		{
			m_map[sym] = val;
		}
		else
		{
			return error("unbound variable: " + stringify(sym));
		}
	}


#if 0
	Environment environment::defPrim(const std::string& n, int id, int minArgs)
	{
		define(symbol::make(n), Primitive(new primitive(id, minArgs, minArgs)));
		return Environment(this);
	}

	Environment environment::defPrim(const std::string& n,
									 int id, int minArgs, int maxArgs)
	{
		define(symbol::make(n), Primitive(new primitive(id, minArgs, maxArgs)));
		return Environment(this);
	}

	bool environment::numberArgsOK(boost::any vars, boost::any vals)
	{
		return ((isEMPTY(vars) && isEMPTY(vals)) ||
				isSymbol(vars) ||
				(isPair(vars) && isPair(vals) &&
				 numberArgsOK(rest(vars), rest(vals))));
	}
#endif
}