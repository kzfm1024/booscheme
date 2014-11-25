//
// syntax.cpp
//

#include <sstream>
#include "boo.h"
#include "boo_objects.h"
#include "syntax.h"

//#include <iostream> // TENTATIVE
#include <stdlib.h> // TENTATIVE for exit()

namespace boo
{
	syntax::syntax(const std::string& name, object* syntax_rules) :
		m_name(name),
		m_syntax_rules(syntax_rules)
	{
	}

	std::string syntax::to_s()
	{
		std::ostringstream stream;
		stream << "#<syntax " << m_name << ">";
		return stream.str();
	}

	object* syntax::expand(object* x)
	{
		object* literal = second(m_syntax_rules);
		object* rules = cdr(cdr(m_syntax_rules));

		DEBUG("syntax::expand() literal", literal);
		DEBUG("syntax::expand() rules", rules);
		
		PDEBUG("syntax::expand() x", x);
		while (!is_null(rules))
		{
			object* rule = car(rules);

			object* pat = first(rule);
			object* tmpl = second(rule);

			DEBUG("syntax::expand pattern", pat);
			DEBUG("syntax::expand template", tmpl);

			if (is_symbol(car(x), name()))
			{
				if (is_symbol(car(pat), name()) || is_symbol(car(pat), "_"))
				{
					environment* env = new environment(NIL(), NIL(), 0);
					if (match(cdr(pat), cdr(x), env))
					{
						std::cout << "match success" << std::endl;

						PDEBUG("syntax::expand template", tmpl);
						std::cout << env->inspect() << std::endl;

						object* expanded = substitute(tmpl, env);
						PDEBUG("syntax::expand expanded", expanded);
					}
				}
			}
			
			rules = cdr(rules);
		}

		exit(0);

		return x; // NOT YET
	}

	bool syntax::match(object* pat, object* x, environment* env)
	{
		PDEBUG("syntax::match pat", pat);
		PDEBUG("syntax::match x", x);

		if (is_null(pat))
		{
			return is_null(x) ? true : false;
		}
		else if (is_pair(pat))
		{
			object* p1 = car(pat);
			if (is_symbol(p1, "..."))
			{
				env->define(p1, x);
				return true;
			}

			if (is_null(x))
			{
				return false;
			}
			else if (is_pair(x))
			{
				object* x1 = car(x);

				if (is_symbol(p1))
				{
					env->define(p1, x1);
					return match(cdr(pat), cdr(x), env);
				}
			}
		}

		return false;
	}

	object* syntax::substitute(object* tmpl, environment* env)
	{
		if (is_null(tmpl))
		{
			return tmpl;
		}
		else // is_pair(tmpl)
		{
			object* head = car(tmpl);
			DEBUG("#0", head);

			if (is_symbol(head))
			{
				symbol* sym = dynamic_cast<symbol*>(head);
				try
				{
					head = env->lookup(sym);
					DEBUG("#1", head);
				}
				catch (const std::exception& e)
				{
					head = car(tmpl);
					DEBUG("#2", head);
				}
			}

			return cons(head, substitute(cdr(tmpl), env));
		}
	}
}
