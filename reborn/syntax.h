//
// syntax.h
//

#pragma once
#ifndef _SYNTAX_H
#define _SYNTAX_H

#include "object.h"
#include "boo.h"

namespace boo
{
    class syntax : public object
    {
	public:
		syntax(const std::string& name, object* syntax_rules);
        virtual ~syntax() {} 
        
        virtual std::string to_s();
        std::string name() { return m_name; }
		object* expand(object* x);

	private:
		bool match(object* pat, object* x, environment* env);
		object* substitute(object* tmpl, environment* env);

        std::string m_name;
		object* m_syntax_rules;
    };
}

#endif // _SYNTAX_H
