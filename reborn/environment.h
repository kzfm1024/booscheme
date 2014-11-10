//
// environment.h
//

#pragma once
#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <map>
#include "object.h"
#include "pair.h"
#include "symbol.h"

namespace boo
{
	class environment : public object
	{
	public:
		environment();
		environment(object* vars, object* vals, environment* parent);
		virtual ~environment() {}

		virtual std::string to_s() { return std::string("#<environment>"); }
		
		object* lookup(symbol* sym);
		object* define(object* var, object* val);
		object* set(object* var, object* val);
/*
		environment defPrim(const std::string& n, int id, int minArgs);
		environment defPrim(const std::string& n, int id, int minArgs, int maxArgs);
		bool numberArgsOK(pair* vars, pair* vals);
*/

	private:
		environment* m_parent;
		std::map<symbol*, object*> m_map;
	};
}

#endif // _ENVIRONMENT_H
