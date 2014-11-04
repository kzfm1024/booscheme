//
// object.h
//

#pragma once
#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include <gc_cpp.h>

namespace booscheme
{
	class object : public gc
	{
	public:
		object() {}
		virtual ~object() {}

		virtual std::string to_s() { return std::string("#<object>"); }
		virtual std::string inspect() { return this->to_s(); } 
	};
}

#endif // _OBJECT_H
