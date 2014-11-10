//
// primitive.h
//

#pragma once
#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "procedure.h"

namespace boo
{
	class primitive : public procedure
	{
	public:
		primitive(int id, int min_args, int max_args);
		virtual ~primitive() {} 

		virtual object* apply(object* args);

	private:
		int m_id;
		int m_min_args;
		int m_max_args;
	};
}

#endif // _PRIMITIVE_H
