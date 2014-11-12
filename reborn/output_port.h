//
// output_port.h
//

#pragma once
#ifndef _OUTPUT_PORT_H
#define _OUTPUT_PORT_H

#include <ostream>
#include "object.h"

namespace boo
{
	class output_port
	{
	public:
		output_port(std::ostream& out) : m_out(out) {}
		virtual ~output_port() {}
    
		void write(const std::string& s);
		void newline();
		void flush();

	private:
		std::ostream& m_out;
	};
}

#endif // _OUTPUT_PORT_H
