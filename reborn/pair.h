//
// pair.h
//

#pragma once
#ifndef _PAIR_H
#define _PAIR_H

#include <string>
#include "object.h"

namespace booscheme
{
	class pair : public object
	{
	public:
		pair(object* car, object* cdr) : m_car(car), m_cdr(cdr) {}
		virtual ~pair() {} 

		virtual std::string to_s();

		object* car() { return m_car; }
		object* cdr() { return m_cdr; }
		object* setcar(object* a) { return m_car = a; }
		object* setcdr(object* b) { return m_cdr = b; }

	private:
		object* m_car;
		object* m_cdr;
	};
}

#endif // _PAIR_H
