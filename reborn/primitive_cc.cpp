//
// primitive_cc.cpp
//

#include <sstream>
#include "boo.h"
#include "boo_types.h"
#include "primitive_cc.h"

namespace boo
{
    primitive_cc::primitive_cc(primitive_func func, int num_args) :
        primitive(func, num_args)
    {
    }

    primitive_cc::primitive_cc(primitive_func func, int min_args, int max_args) :
        primitive(func, min_args, max_args)
    {
    }

    std::string primitive_cc::to_s()
    {
        std::ostringstream stream;
        stream << "#<primitive_cc " << m_name << ">";
        return stream.str();
    }

	object* primitive_cc::apply(object* args)
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

        return cc->apply((*m_func)(args));
    } 
}
