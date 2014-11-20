//
// continuation.cpp
//

#include <assert.h>
#include "boo.h"
#include "boo_types.h"
#include "continuation.h"

namespace boo
{
	continuation_define::continuation_define(object* var, environment* env, continuation* cc) : 
		continuation(1),
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
		continuation(1),
		m_out(out),
		m_quoted(quoted)
	{
	}

	object* continuation_write::apply(object* val)
	{
		pdebug(val);
		return boo::write(val, m_out, m_quoted);
	}

	continuation_procedure::continuation_procedure(object* args, environment* env, continuation* cc) :
		continuation(1),
		m_args(args),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_procedure::apply(object* proc)
	{
		pdebug("continuation_procedure", proc);

			if (is_closure(proc))
		{
			closure* cl = dynamic_cast<closure*>(proc);
			continuation* cc2 = new continuation_closure(cl->body(), cl->params(), cl->env(), m_cc);
			return evlist_cc(m_args, m_env, cc2);
		}
		else
		{
			pdebug(proc);
			primitive_cc* prim = dynamic_cast<primitive_cc*>(proc);
			assert(prim);
			continuation* cc2 = new continuation_primitive(prim, m_cc);
			return evlist_cc(m_args, m_env, cc2);
		}
	}


	continuation_closure::continuation_closure(object* body, object* params, environment* env, continuation* cc) :
		continuation(1),
		m_body(body),
		m_params(params),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_closure::apply(object* args)
	{
		args = car(args);
		environment* env = new environment(m_params, args, m_env);
		return eval_cc(m_body, env, m_cc);
	}

	continuation_primitive::continuation_primitive(primitive_cc* prim, continuation* cc) :
		continuation(1),
		m_prim(prim),
		m_cc(cc)
	{
	}

	object* continuation_primitive::apply(object* args)
	{
		pdebug(m_prim);
		pdebug(args);
		return m_prim->apply(cons(m_cc, args));
	}

	continuation_apply::continuation_apply(continuation* cc) : 
		continuation(1),
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
		continuation(1),
		m_lst(lst),
		m_env(env),
		m_cc(cc)
	{
	}

	object* continuation_evlist::apply(object* args)
	{
		pdebug("continuation_evlist::apply", args);

		if (is_null(args))
		{
			return m_cc->apply(args); // FIXME
		}
		else
		{
			continuation* cc2 = new continuation_evlist2(args, m_cc);
			return evlist_cc(m_lst, m_env, cc2);
		}

	} 

	continuation_evlist2::continuation_evlist2(object* x, continuation* cc) :
		continuation(1),
		m_x(x),
		m_cc(cc)
	{
	}

	object* continuation_evlist2::apply(object* y)
	{
		pdebug("continuation_evlist2::apply m_x", m_x);
		pdebug("continuation_evlist2::apply y", y);

		if (is_null(y))
		{
			return m_cc->apply(cons(m_x, y));
		}
		else
		{
			return m_cc->apply(cons(m_x, y));
		}
	}
}
