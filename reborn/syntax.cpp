//
// syntax.cpp
//

#include <sstream>
#include "boo.h"
#include "boo_objects.h"
#include "syntax.h"

namespace boo
{
	syntax::syntax(const std::string& name, object* syntax_rules) :
		m_name(name),
		m_syntax_rules(syntax_rules)
	{
	}

	std::string syntax::to_s()
	{
		std::ostringstream stream;
		stream << "#<syntax " << m_name << ">";
		return stream.str();
	}

	object* syntax::expand(object* x)
	{
		return x; // NOT YET
	}
}
