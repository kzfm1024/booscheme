//
// number.cpp
//

#include <sstream>
#include "number.h"

namespace boo
{
	std::string number::to_s()
	{
		std::ostringstream stream;
		stream << m_num;
		return stream.str();
	}
}
