//
// primitive_cc.h
//

#pragma once
#ifndef _PRIMITIVE_CC_H
#define _PRIMITIVE_CC_H

#include "primitive.h"

namespace boo
{
	class primitive_cc : public primitive
	{
	public:
		primitive_cc(primitive_func func, int num_args);
		primitive_cc(primitive_func func, int min_args, int max_args);
		virtual ~primitive_cc() {} 
		
		virtual object* apply(object* args);
	};
}

#endif // _PRIMITIVE_CC_H
