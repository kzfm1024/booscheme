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
    class environment;
    class input_port;
    class output_port;
    class eof;
    class number;

    boolean* TRUE();
    boolean* FALSE();
    boolean* BOOLEAN(bool x);
    null* NIL();
    eof* EOF();
    misc* UNDEF();
    misc* error(const std::string& message);
    misc* warn(const std::string& message);

    object* cons(object* a, object* b);
    object* cons(object* a, object* b);
    object* car(object* x);
    object* cdr(object* x);
    object* setcar(object* x, object* a);
    object* setcdr(object* x, object* b);

    object* first(object* x);
    object* second(object* x);
    object* third(object* x);

    object* list(object* a, object* b);
    int length(object* x);

    bool truth(object* x);
    bool is_null(object* x);
    bool is_pair(object* x);
    bool is_number(object* x);
    bool is_string(object* x);
    bool is_character(object* x);
    bool is_symbol(object* x);
    bool is_symbol(object* x, const std::string& s);
    bool is_procedure(object* x, const std::string& s);
    bool is_closure(object* x);
    bool is_eof(object* x);
    number* to_number(const std::string& s);

    std::string stringify(object* x, bool quoted);
    std::string stringify(object* x);

    object* write(object* x, output_port* port, bool quoted);

    void pdebug(object* x);
    void pdebug(const std::string& msg, object* x);

    object* eval(object* x, environment* env);
    object* evlist(object* lst, environment* env);

    class primitive_cc;
    object* eval_cc(object* x, environment* env, primitive* cc);
    // object* evlist(object* lst, environment* env);
}

#endif // _BOO_H
