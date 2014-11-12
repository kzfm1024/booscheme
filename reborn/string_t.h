//
// string_t.h
//

#pragma once
#ifndef _STRING_T_H
#define _STRING_T_H

#include "object.h"

namespace boo
{
	class string_t : public object
	{
	public:
		string_t(const std::string& str) : m_str(str) {}
		virtual ~string_t() {} 

		virtual std::string to_s() { return m_str; }
                
	private:
		std::string m_str;
	};
}

#endif // _STRING_T_H
