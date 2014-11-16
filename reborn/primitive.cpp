//
// primitive.cpp
//

#include <sstream>
#include "boo.h"
#include "boo_types.h"
#include "primitive.h"

namespace boo
{
	primitive::primitive(primitive_func func, int num_args) :
		m_func(func),
		m_min_args(num_args),
		m_max_args(num_args)
	{
	}

	primitive::primitive(primitive_func func, int min_args, int max_args) :
		m_func(func),
		m_min_args(min_args),
		m_max_args(max_args)
	{
	}

	std::string primitive::to_s()
	{
		std::ostringstream stream;
		stream << "#<primitive " << m_name << ">";
		return stream.str();
	}

	object* primitive::apply(object* args)
	{
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

		return (*m_func)(args);
	} 
}
