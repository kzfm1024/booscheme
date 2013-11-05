#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Symbol class
#
Symbol = str

#
# Env class
#
class Env(dict):
    """An environment: a dict of {'var':val} pairs, with an outer Env."""
    def __init__(self, parms = (), args = (), outer = None):
        self.update(zip(parms, args))
        self.outer = outer
    def find(self, var):
        "Find the innermost Env where var appears."
        return self if var in self else self.outer.find(var)

#
# Hop class
#
class Hop:
    """A tiny scheme interpreter"""
    def __init__(self):
        self.initial_env = self.add_primitives(Env())
    def add_primitives(self, env):
        """Add some Scheme standard procedures to an environment."""
        import math, operator as op
        env.update(vars(math)) # sin, sqrt, ...
        env.update({ '+': op.add,
                     '-': op.sub,
                     '*': op.mul,
                     '/': op.div,
                     'not': op.not_,
                     '>': op.gt,
                     '<': op.lt,
                     '>=': op.ge,
                     '<=': op.le,
                     '=': op.eq, 
                     'equal?': op.eq,
                     'eq?': op.is_,
                     'length': len,
                     'cons': lambda x, y: [x] + y,
                     'car': lambda x: x[0],
                     'cdr': lambda x: x[1:],
                     'append': op.add,  
                     'list': lambda *x: list(x),
                     'list?': lambda x: isinstance(x, list), 
                     'null?': lambda x: x==[],
                     'symbol?': lambda x: isinstance(x, Symbol) })
        return env
    def eval(self, x, env):
        """Evaluate an expression in an environment."""
        if isinstance(x, Symbol):             # variable reference
            return env.find(x)[x]
        elif not isinstance(x, list):         # constant literal
            return x                
        elif x[0] == 'quote':
            (_, exp) = x
            return x
        elif x[0] == 'if':                    # (if test conseq alt)
            (_, test, conseq, alt) = x
            return self.eval ((conseq if self.eval(test, env) else alt), env)
        elif x[0] == 'set!':                  # (set! var exp)
            (_, var, exp) = x
            env.find(var)[var] = self.eval(exp, env)
        elif x[0] == 'define':                # (define var exp)
            (_, var, exp) = x
            env[var] = self.eval(exp, env)
        elif x[0] == 'lambda':                # (lambda (var*) exp)
            (_, vars, exp) = x
            return lambda *args: self.eval(exp, Env(vars, args, env))
        elif x[0] == 'begin':                 # (begin exp*)
            for exp in x[1:]:
                val = self.eval(exp, env)
                return val
        else:                                 # (proc exp*)
            exps = [self.eval(exp, env) for exp in x]
            proc = exps.pop(0)
            return proc(*exps)
        
if __name__ == '__main__':
    hop = Hop()
#    print hop.initial_env
    print hop.eval(1, hop.initial_env)
    print hop.eval('+', hop.initial_env)
    print hop.eval(['+', 1, 2], hop.initial_env)
