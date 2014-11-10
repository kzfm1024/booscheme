//
// pair.cpp
//

#include <sstream>
#include "pair.h"

namespace boo
{
	std::string pair::to_s()
	{
		std::ostringstream stream;
		stream << "(" << m_car->to_s() << " . " << m_cdr->to_s() << ")";
		return stream.str();
	}
}
