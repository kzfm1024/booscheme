//
// procedure.h
//

#pragma once
#ifndef _PROCEDURE_H
#define _PROCEDURE_H

#include "object.h"
#include "boo.h"

namespace boo
{
	class procedure : public object
	{
	public:
	    procedure() : m_name("anonymous procedure") {}
		virtual ~procedure() {} 
		
		virtual std::string to_s() { return m_name; }
		std::string name() { return m_name; }
		void set_name(const std::string& name) { m_name = name; }

	protected:
		std::string m_name;
	};
}

#endif // _PROCEDURE_H
