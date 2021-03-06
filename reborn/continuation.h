//
// continuation.h
//

#pragma once
#ifndef _CONTINUATION_H
#define _CONTINUATION_H

#include "environment.h"
#include "procedure.h"
#include "primitive_cc.h"
#include "output_port.h"

namespace boo
{
    class continuation : public procedure
    {
    public:
        //continuation(const std::string& name) { set_name(name); }
	continuation(const std::string& name) : procedure(name) {}
        ~continuation() {}

        virtual object* apply(object* val) { return val; }
    };

    class continuation_define : public continuation
    {
    public:
        continuation_define(object* var, environment* env, continuation* cc);
        virtual ~continuation_define() {} 
        
        virtual object* apply(object* val);

    private:
        object* m_var;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_write : public continuation
    {
    public:
        continuation_write(output_port* out, bool quoted);
        virtual ~continuation_write() {} 
        
        virtual object* apply(object* val);

    private:
        output_port* m_out;
        bool m_quoted;
    };

    class continuation_begin : public continuation
    {
    public:
        continuation_begin(object* rest, environment* env, continuation* cc);
        virtual ~continuation_begin() {}

        virtual object* apply(object* val);

    private:
        object* m_rest;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_procedure : public continuation
    {
    public:
        continuation_procedure(object* args, environment* env, continuation* cc);
        virtual ~continuation_procedure() {} 

        virtual object* apply(object* val);

    private:
        object* m_args;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_closure : public continuation
    {
    public:
        continuation_closure(object* body, object* params, environment* env, continuation* cc);
        virtual ~continuation_closure() {} 

        virtual object* apply(object* val);

    private:
        object* m_body;
        object* m_params;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_primitive : public continuation
    {
    public:
        continuation_primitive(primitive_cc* prim, continuation* cc);
        virtual ~continuation_primitive() {} 

        virtual object* apply(object* args);

    private:
        primitive_cc* m_prim;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_apply : public continuation
    {
    public:
        continuation_apply(continuation* cc);
        virtual ~continuation_apply() {} 
        
        virtual object* apply(object* val);

    private:
        continuation* m_cc;
    };

    class continuation_evlist : public continuation
    {
    public:
        continuation_evlist(object* lst, environment* env, continuation* cc);
        virtual ~continuation_evlist() {} 
        
        virtual object* apply(object* val);

    private:
        object* m_lst;
        environment* m_env;
        continuation* m_cc;
    };

    class continuation_evlist2 : public continuation
    {
    public:
        continuation_evlist2(object* x, continuation* cc);
        virtual ~continuation_evlist2() {} 
        
        virtual object* apply(object* y);

    private:
        object* m_x;
        continuation* m_cc;
    };

}

#endif // _CONTINUATION_H
