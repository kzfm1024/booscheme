//
// primitive_cc.cpp
//

#include "boo.h"
#include "primitive_cc.h"

namespace boo
{
	primitive_cc_define::primitive_cc_define(object* var, environment* env, primitive* cc) : 
		primitive(0, 1),
		m_var(var),
		m_env(env),
		m_cc(cc)
	{
	}

	object* primitive_cc_define::apply(object* val)
	{
		object* var = m_env->define(m_var, val);
		return m_cc->apply(var);
	} 

	primitive_cc_write::primitive_cc_write(output_port* out, bool quoted) :
		primitive(0, 1),
		m_out(out),
		m_quoted(quoted)
	{
	}

	object* primitive_cc_write::apply(object* x)
	{
		boo::write(x, m_out, m_quoted);
	}
}
