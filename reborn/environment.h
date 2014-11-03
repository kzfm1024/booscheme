//
// environment.h
//

#pragma once
#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <boost/any.hpp>

namespace booscheme
{
	class environment
	{
	public:
		environment();
		environment(boost::any vars, boost::any vals, environment parent);
		~environment() {}

		boost::any lookup(Symbol sym);
		boost::any define(boost::any var, boost::any val);
		boost::any set(boost::any var, boost::any val);
		environment defPrim(const std::string& n, int id, int minArgs);
		environment defPrim(const std::string& n, int id, int minArgs, int maxArgs);
		bool numberArgsOK(boost::any vars, boost::any vals);

	private:
		boost::any vars;
		boost::any vals;
		environment parent;
	};
}

#endif // _ENVIRONMENT_H
