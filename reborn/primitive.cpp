//
// primitive.cpp
//

#include <sstream>
#include "primitive.h"

#include "number.h" // FIXME
#include "boo.h" // FIXME

namespace boo
{
	primitive::primitive(int id, int min_args, int max_args) :
		m_id(id),
		m_min_args(min_args),
		m_max_args(max_args)
	{
	}

	object* primitive::apply(object* args)
	{
		// return UNDEF();

		number* n1 = dynamic_cast<number*>(first(args));
		number* n2 = dynamic_cast<number*>(second(args));
		return new number(n1->to_i() + n2->to_i());
	} 
}
