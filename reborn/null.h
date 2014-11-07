//
// null.h
//

#pragma once
#ifndef _NULL_H
#define _NULL_H

#include <string>
#include "object.h"

namespace boo
{
	class null : public object
	{
	public:
		null() {}
		virtual ~null() {} 

		virtual std::string to_s() { return std::string("()"); }
	};
}

#endif // _NULL_H
