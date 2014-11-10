//
// boolean.h
//

#pragma once
#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#include <map>
#include "object.h"

namespace boo
{
	class boolean : public object
	{
	public:
		boolean(bool val) : m_val(val) {}
		virtual ~boolean() {} 

		virtual std::string to_s() { return m_val ? std::string("#t") : std::string("#f"); }
		virtual std::string inspect() { return this->to_s(); }
		
	private:
		bool m_val;
	};
}

#endif // _BOOLEAN_H
