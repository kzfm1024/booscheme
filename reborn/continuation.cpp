//
// continuation.cpp
//

#include <assert.h>
#include "boo.h"
#include "boo_objects.h"
#include "continuation.h"

namespace boo
{
	continuation_define::continuation_define(object* var, environment* env, continuation* cc) : 
		continuation("continuation_define"),
		m_var(var),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_define::apply(object* val)
	{
		object* var = m_env->define(m_var, val);
		return m_cc->apply(var);
	} 

	continuation_write::continuation_write(output_port* out, bool quoted) :
		continuation("continuation_write"),
		m_out(out),
		m_quoted(quoted)
	{
	}

	object* continuation_write::apply(object* val)
	{
		DEBUG("continuation_write::apply", val);
		return boo::write(val, m_out, m_quoted);
	}

	continuation_begin::continuation_begin(object* rest, environment* env, continuation* cc) :
		continuation("continuation_begin"),
		m_rest(rest),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_begin::apply(object* val)
	{
		DEBUG("continuation_begin::apply val", val);
		DEBUG("continuation_begin::apply m_rest", m_rest);
		if (is_null(m_rest))
		{
			return m_cc->apply(val);
		}
		else
		{
			continuation* cc2 = new continuation_begin(cdr(m_rest), m_env, m_cc);
			return eval_cc(car(m_rest), m_env, cc2);
		}
	}

	continuation_procedure::continuation_procedure(object* args, environment* env, continuation* cc) :
		continuation("continuation_procedure"),
		m_args(args),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_procedure::apply(object* proc)
	{
		DEBUG("continuation_procedure::apply", proc);
		
		if (is_closure(proc))
		{
			closure* cl = dynamic_cast<closure*>(proc);
			continuation* cc2 = new continuation_closure(cl->body(), cl->params(), cl->env(), m_cc);
			return evlist_cc(m_args, m_env, cc2);
		}
		else if (is_continuation(proc)) // call-with-current-continuation
		{
			continuation* cc = dynamic_cast<continuation*>(proc);
			return cc->apply(car(m_args));
		}
		else
		{
			primitive_cc* prim = dynamic_cast<primitive_cc*>(proc);
			assert(prim);
			continuation* cc2 = new continuation_primitive(prim, m_cc);
			return evlist_cc(m_args, m_env, cc2);
		}
	}


	continuation_closure::continuation_closure(object* body, object* params, environment* env, continuation* cc) :
		continuation("continuation_closure"),
		m_body(body),
		m_params(params),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_closure::apply(object* args)
	{
		environment* env = new environment(m_params, args, m_env);
		return eval_cc(m_body, env, m_cc);
	}

	continuation_primitive::continuation_primitive(primitive_cc* prim, continuation* cc) :
		continuation("continuation_primitive"),
		m_prim(prim),
		m_cc(cc)
	{
	}

	object* continuation_primitive::apply(object* args)
	{
		DEBUG("continuation_primitive::apply m_prim", m_prim);
		DEBUG("continuation_primitive::apply m_cc", m_cc);
		DEBUG("continuation_primitive::apply args", args);
		return m_prim->apply(cons(m_cc, args));
	}

	continuation_apply::continuation_apply(continuation* cc) : 
		continuation("continuation_apply"),
		m_cc(cc)
	{
	}

	object* continuation_apply::apply(object* fn_and_args)
	{
		fn_and_args = car(fn_and_args);
		primitive_cc* fn = dynamic_cast<primitive_cc*>(car(fn_and_args));
		assert(fn);
		object* args = cdr(fn_and_args);
		return fn->apply(list(cons(m_cc, args)));
	}

	continuation_evlist::continuation_evlist(object* lst, environment* env, continuation* cc) : 
		continuation("continuation_evlist"),
		m_lst(lst),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_evlist::apply(object* args)
	{
		DEBUG("continuation_evlist::apply", args);
		
		if (is_null(args))
		{
			return m_cc->apply(args);
		}
		else
		{
			continuation* cc2 = new continuation_evlist2(args, m_cc);
			return evlist_cc(m_lst, m_env, cc2);
		}

	} 

	continuation_evlist2::continuation_evlist2(object* x, continuation* cc) :
		continuation("continuation_evlist2"),
		m_x(x),
		m_cc(cc)
	{
	}

	object* continuation_evlist2::apply(object* y)
	{
		DEBUG("continuation_evlist2::apply m_x", m_x);
		DEBUG("continuation_evlist2::apply y", y);
		DEBUG("continuation_evlist2::apply m_cc", m_cc);
		return m_cc->apply(cons(m_x, y));
	}
}
