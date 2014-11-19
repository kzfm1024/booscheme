//
// primitive_call_cc.h
//

#pragma once
#ifndef _PRIMITIVE_CALL_CC_H
#define _PRIMITIVE_CALL_CC_H

#include "primitive_cc.h"

namespace boo
{
	class primitive_call_cc : public primitive_cc
	{
	public:
		primitive_call_cc();
		virtual ~primitive_call_cc() {} 
		
		virtual object* apply(object* args);
	};
}

#endif // _PRIMITIVE_CALL_CC_H
