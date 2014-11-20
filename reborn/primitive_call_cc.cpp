//
// primitive_call_cc.cpp
//

#include <sstream>
#include "boo.h"
#include "boo_types.h"
#include "primitive_call_cc.h"

namespace boo
{
    primitive_call_cc::primitive_call_cc() : primitive_cc(0, 1)
    {
    }

	object* primitive_call_cc::apply(object* args)
	{
		continuation* cc = dynamic_cast<continuation*>(car(args));
		args = cdr(args);

        int num_args = length(args);
        if (num_args < m_min_args)
        {
            std::stringstream msg;
            msg << "too few args, " << num_args << ", for ";
            msg << m_name << ": " << stringify(args);
            return error(msg.str());
        }
        else if (num_args > m_max_args)
        {
            std::stringstream msg;
            msg << "too many args, " << num_args << ", for ";
            msg << m_name << ": " << stringify(args);
            return error(msg.str());
        }

		closure* cl = dynamic_cast<closure*>(car(args));		
		environment* env = new environment(cl->params(), list(cc), cl->env());
		return eval_cc(cl->body(), env, cc);
    } 
}
