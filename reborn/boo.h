//
// boo.h
//

#pragma once
#ifndef _BOO_H
#define _BOO_H

#include <string>

namespace boo
{
	class object;
	class boolean;
	class null;
	class misc;
	class pair;

    boolean* TRUE();
    boolean* FALSE();
    boolean* BOOLEAN(bool x);
    null* NIL();
	object* UNDEF();
	misc* error(const std::string& message);
	misc* warn(const std::string& message);

	pair* cons(object* a, object* b);
    object* car(object* x);
	object* cdr(object* x);
	object* setcar(object* x, object* a);
	object* setcdr(object* x, object* b);

    bool is_null(object* x);
    bool is_pair(object* x);
    bool is_number(object* x);
    bool is_character(object* x);
    bool is_number(object* x);
	bool is_symbol(object* x);
	bool is_symbol(object* x, const std::string& s);
	bool is_procedure(object* x, const std::string& s);

	std::string stringify(object* x, bool quoted);
	std::string stringify(object* x);

	void print(object* x);
	void print(const std::string& msg, object* x);
}

#endif // _BOO_H
