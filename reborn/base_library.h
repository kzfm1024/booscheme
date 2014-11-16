//
// base_library.h
//

#pragma once
#ifndef _BASE_LIBRARY_H
#define _BASE_LIBRARY_H

#include "object.h"
#include "environment.h"

namespace boo
{
	class base_library
    {
	private:
		base_library() {}
		base_library(const base_library& lib) {}
		~base_library() {}
		base_library& operator=(const base_library& lib) {}

	public:
		static void install(environment* env);
		
	private:
		static object* cons(object*);
		static object* plus(object*);
		static object* times(object*);
    };
}

#endif // _BASE_LIBRARY_H
