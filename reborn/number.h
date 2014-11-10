//
// number.h
//

#pragma once
#ifndef _NUMBER_H
#define _NUMBER_H

#include <stdint.h>
#include "object.h"

namespace boo
{
	class number : public object
	{
	public:
	    number(int64_t num) : m_num(num) {}
		virtual ~number() {} 

		virtual std::string to_s();
		int64_t to_i() { return m_num; }
		
	private:
		int64_t m_num;
	};
}

#endif // _NUMBER_H
