//
// eof.h
//

#pragma once
#ifndef _EOF_H
#define _EOF_H

#include <string>
#include "object.h"

namespace boo
{
	class eof : public object
	{
	public:
		eof() {}
		virtual ~eof() {} 

		virtual std::string to_s() { return std::string("#<eof>"); }
	};
}

#endif // _EOF_H
