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

		virtual std::string inspect() { return std::string("#<object>"); }
	};
}

#endif // _OBJECT_H
