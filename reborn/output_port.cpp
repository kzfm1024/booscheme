//
// output_port.cpp
//

#include "output_port.h"

namespace boo
{
	void output_port::write(const std::string& s)
	{
		m_out << s;
	}

	void output_port::newline()
	{
		m_out << std::endl;
	}

	void output_port::flush()
	{
		m_out << std::flush;
	}
}
