//
// input_port.h
//

#pragma once
#ifndef _INPUT_PORT_H
#define _INPUT_PORT_H

#include <istream>
#include "object.h"

namespace boo
{
	class input_port : public object
	{
	public:
		input_port(std::istream& is);
		virtual ~input_port() {}

		virtual std::string to_s() { return std::string("#<input_port>"); }

		object* read_character();
		object* peek_character();
		object* read();

	private:
		int peek_char();
		int push_char(int ch);
		int pop_char();
		object* next_token();
		// object* readTail();
		// object* nextToken();

		bool          m_is_pushed_token;;
		bool          m_is_pushed_char;
		object*       m_pushed_token;
		int           m_pushed_char;
		std::istream& m_in;
		std::string   m_buf;
	};
}

#endif // _INPUT_PORT_H
