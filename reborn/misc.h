//
// misc.h
//

#pragma once
#ifndef _MISC_H
#define _MISC_H

#include "object.h"

namespace boo
{
	class misc : public object
	{
	public:
	    misc(const std::string& name) : m_name(name) {}
		virtual ~misc() {} 

		virtual std::string to_s() { return m_name; }

	private:
		std::string m_name;
	};
}

#endif // _MISC_H
