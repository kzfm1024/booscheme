//
// primitive_cc.h
//

#pragma once
#ifndef _PRIMITIVE_CC_H
#define _PRIMITIVE_CC_H

#include "environment.h"
#include "primitive.h"
#include "output_port.h"

namespace boo
{
	class primitive_cc_define : public primitive
	{
	public:
		primitive_cc_define(object* var, environment* env, primitive* cc);
		virtual ~primitive_cc_define() {} 
		
		virtual object* apply(object* args);

	private:
		object* m_var;
		environment* m_env;
		primitive* m_cc;
	};

	class primitive_cc_write : public primitive
	{
	public:
		primitive_cc_write(output_port* out, bool quoted);
		virtual ~primitive_cc_write() {} 
		
		virtual object* apply(object* args);

	private:
		output_port* m_out;
		bool m_quoted;
	};
}

#endif // _PRIMITIVE_CC_H
