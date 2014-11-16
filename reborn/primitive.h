//
// primitive.h
//

#pragma once
#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "procedure.h"

namespace boo
{
	typedef object* (*primitive_func)(object*);
	
	class primitive : public procedure
	{
	public:
		primitive(primitive_func func, int num_args);
		primitive(primitive_func func, int min_args, int max_args);
		virtual ~primitive() {} 
		
		virtual std::string to_s();
		virtual object* apply(object* args);

	private:
		primitive_func m_func;
		int m_min_args;
		int m_max_args;
	};
}

#endif // _PRIMITIVE_H
