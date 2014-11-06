//
// empty.h
//

#pragma once
#ifndef _EMPTY_H
#define _EMPTY_H

#include <string>
#include "object.h"

namespace booscheme
{
	class empty : public object
	{
	public:
		empty() {}
		virtual ~empty() {} 

		virtual std::string to_s() { return std::string("()"); }
	};
}

#endif // _EMPTY_H
