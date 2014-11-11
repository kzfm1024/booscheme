//
// closure.h
//

#pragma once
#ifndef _CLOSURE_H
#define _CLOSURE_H

#include "procedure.h"

namespace boo
{
	class closure : public procedure
	{
	public:
	    closure(object* params, object* body, environment* env);
		virtual ~closure() {} 

		virtual std::string to_s();

		object* params()   { return m_params; }
		object* body()     { return m_body;   }
		environment* env() { return m_env;    }

	private:
		object* m_params;
		object* m_body;
		environment* m_env;
	};
}

#endif // _CLOSURE_H
