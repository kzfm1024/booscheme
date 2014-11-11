//
// closure.cpp
//

#include <sstream>
#include "closure.h"

namespace boo
{
	closure::closure(object* params, object* body, environment* env) :
		m_params(params),
		m_body(body),
		m_env(env)
	{
	}
	
	std::string closure::to_s()
	{
		std::ostringstream stream;
		stream << "#<closure ";
		stream << m_name;
		stream << ">";
		return stream.str();
	}
}
