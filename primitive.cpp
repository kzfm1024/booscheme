//
// primitive.cpp
//

#include <limits.h> // for INT_MAX
#include "booscheme.h"

/*

car, cdr, atom?, cons, eq?
not
pair?

(define (atom? x) (not (pair? x)))

 */

static const int EQ = 0, LT = 1, GT = 2, GE = 3, LE = 4,
     ABS = 5, EOF_OBJECT = 6, EQQ = 7, EQUALQ = 8, FORCE = 9,
     CAR = 10, FLOOR = 11,  CEILING = 12, CONS = 13, 
     DIVIDE= 14, LENGTH = 15, LIST = 16, LISTQ = 17, APPLY = 18,
     MAX = 19, MIN = 20, MINUS = 21, NEWLINE = 22, 
     NOT = 23, NULLQ = 24, NUMBERQ = 25, PAIRQ = 26, PLUS = 27, 
     PROCEDUREQ = 28, READ = 29, CDR = 30, ROUND = 31, SECOND = 32, 
     SYMBOLQ = 33, TIMES = 34, TRUNCATE = 35, WRITE = 36, APPEND = 37,
     BOOLEANQ = 38, SQRT = 39, EXPT = 40, REVERSE = 41, ASSOC = 42, 
     ASSQ = 43, ASSV = 44, MEMBER = 45, MEMQ = 46, MEMV = 47, EQVQ = 48,
     LISTREF = 49, LISTTAIL = 50, STRINQ = 51, MAKESTRING = 52, STRING = 53,
     STRINGLENGTH = 54, STRINGREF = 55, STRINGSET = 56, SUBSTRING = 57, 
     STRINGAPPEND = 58, STRINGTOLIST = 59, LISTTOSTRING = 60, 
     SYMBOLTOSTRING = 61, STRINGTOSYMBOL = 62, EXP = 63, LOG = 64, SIN = 65,
     COS = 66, TAN = 67, ACOS = 68, ASIN = 69, ATAN = 70, 
     NUMBERTOSTRING = 71, STRINGTONUMBER = 72, CHARQ = 73,
     CHARALPHABETICQ = 74, CHARNUMERICQ = 75, CHARWHITESPACEQ = 76,
     CHARUPPERCASEQ = 77, CHARLOWERCASEQ = 78, CHARTOINTEGER = 79,
     INTEGERTOCHAR = 80, CHARUPCASE = 81, CHARDOWNCASE = 82, STRINGQ = 83,
     VECTORQ = 84, MAKEVECTOR = 85, VECTOR = 86, VECTORLENGTH = 87,
     VECTORREF = 88, VECTORSET = 89, LISTTOVECTOR = 90, MAP = 91, 
     FOREACH = 92, CALLCC = 93, VECTORTOLIST = 94, LOAD = 95, DISPLAY = 96,
     INPUTPORTQ = 98, CURRENTINPUTPORT = 99, OPENINPUTFILE = 100, 
     CLOSEINPUTPORT = 101, OUTPUTPORTQ = 103, CURRENTOUTPUTPORT = 104,
     OPENOUTPUTFILE = 105, CLOSEOUTPUTPORT = 106, READCHAR = 107,
     PEEKCHAR = 108, EVAL = 109, QUOTIENT = 110, REMAINDER = 111,
     MODULO = 112, THIRD = 113, EOFOBJECTQ = 114, GCD = 115, LCM = 116, 
     CXR = 117, ODDQ = 118, EVENQ = 119, ZEROQ = 120, POSITIVEQ = 121,
     NEGATIVEQ = 122, 
     CHARCMP = 123 /* to 127 */, CHARCICMP = 128 /* to 132 */,
     STRINGCMP = 133 /* to 137 */, STRINGCICMP = 138 /* to 142 */,
     EXACTQ = 143, INEXACTQ = 144, INTEGERQ = 145,
     CALLWITHINPUTFILE = 146, CALLWITHOUTPUTFILE = 147
    ;

//////////////// Extensions ////////////////

static const int NEW = -1, CLASS = -2, METHOD = -3, EXIT = -4,
    SETCAR = -5, SETCDR = -6, TIMECALL = -11, MACROEXPAND = -12,
    ERROR = -13, LISTSTAR = -14
    ;

primitive::primitive(int id, int minArgs, int maxArgs)
    : id(id), minArgs(minArgs), maxArgs(maxArgs)
{
}

Environment
primitive::installPrimitives(Environment env)
{
    int n = INT_MAX;

    env->defPrim("*",       	TIMES,     0, n);
    env->defPrim("*",       	TIMES,     0, n);
    env->defPrim("+",       	PLUS,      0, n);
    env->defPrim("-",       	MINUS,     1, n);
    env->defPrim("/",       	DIVIDE,    1, n);
    env->defPrim("<",       	LT,        2, n);
    env->defPrim("<=",      	LE,        2, n);
    env->defPrim("=",       	EQ,        2, n);
    env->defPrim(">",       	GT,        2, n);
    env->defPrim(">=",      	GE,        2, n);
    env->defPrim("abs",     	ABS,       1);
    env->defPrim("acos",    	ACOS,      1);
    env->defPrim("append",         APPEND,    0, n);
    env->defPrim("apply",   	APPLY,     2, n);
    env->defPrim("asin",    	ASIN,      1);
    env->defPrim("assoc",   	ASSOC,     2);
    env->defPrim("assq",    	ASSQ,      2);
    env->defPrim("assv",    	ASSV,      2);
    env->defPrim("atan",    	ATAN,      1);
    env->defPrim("boolean?",	BOOLEANQ,  1);
    env->defPrim("caaaar",         CXR,       1);
    env->defPrim("caaadr",         CXR,       1);
    env->defPrim("caaar",          CXR,       1);
    env->defPrim("caadar",         CXR,       1);
    env->defPrim("caaddr",         CXR,       1);
    env->defPrim("caadr",          CXR,       1);
    env->defPrim("caar",           CXR,       1);
    env->defPrim("cadaar",         CXR,       1);
    env->defPrim("cadadr",         CXR,       1);
    env->defPrim("cadar",          CXR,       1);
    env->defPrim("caddar",         CXR,       1);
    env->defPrim("cadddr",         CXR,       1);
    env->defPrim("caddr",     	THIRD,     1);
    env->defPrim("cadr",  	        SECOND,    1);
    env->defPrim("call-with-current-continuation",        CALLCC,    1);
    env->defPrim("call-with-input-file", CALLWITHINPUTFILE, 2);
    env->defPrim("call-with-output-file", CALLWITHOUTPUTFILE, 2);
    env->defPrim("car",     	CAR,       1);
    env->defPrim("cdaaar",         CXR,       1);
    env->defPrim("cdaadr",         CXR,       1);
    env->defPrim("cdaar",          CXR,       1);
    env->defPrim("cdadar",         CXR,       1);
    env->defPrim("cdaddr",         CXR,       1);
    env->defPrim("cdadr",          CXR,       1);
    env->defPrim("cdar",           CXR,       1);
    env->defPrim("cddaar",         CXR,       1);
    env->defPrim("cddadr",         CXR,       1);
    env->defPrim("cddar",          CXR,       1);
    env->defPrim("cdddar",         CXR,       1);
    env->defPrim("cddddr",         CXR,       1);
    env->defPrim("cdddr",          CXR,       1);
    env->defPrim("cddr",           CXR,       1);
    env->defPrim("cdr",     	CDR,       1);
    env->defPrim("char->integer",  CHARTOINTEGER,      1);
    env->defPrim("char-alphabetic?",CHARALPHABETICQ,      1);
    env->defPrim("char-ci<=?",     CHARCICMP+LE, 2);
    env->defPrim("char-ci<?" ,     CHARCICMP+LT, 2);
    env->defPrim("char-ci=?" ,     CHARCICMP+EQ, 2);
    env->defPrim("char-ci>=?",     CHARCICMP+GE, 2);
    env->defPrim("char-ci>?" ,     CHARCICMP+GT, 2);
    env->defPrim("char-downcase",  CHARDOWNCASE,      1);
    env->defPrim("char-lower-case?",CHARLOWERCASEQ,      1);
    env->defPrim("char-numeric?",  CHARNUMERICQ,      1);
    env->defPrim("char-upcase",    CHARUPCASE,      1);
    env->defPrim("char-upper-case?",CHARUPPERCASEQ,      1);
    env->defPrim("char-whitespace?",CHARWHITESPACEQ,      1);
    env->defPrim("char<=?",        CHARCMP+LE, 2);
    env->defPrim("char<?",         CHARCMP+LT, 2);
    env->defPrim("char=?",         CHARCMP+EQ, 2);
    env->defPrim("char>=?",        CHARCMP+GE, 2);
    env->defPrim("char>?",         CHARCMP+GT, 2);
    env->defPrim("char?",   	CHARQ,     1);
    env->defPrim("close-input-port", CLOSEINPUTPORT, 1);
    env->defPrim("close-output-port", CLOSEOUTPUTPORT, 1);
    env->defPrim("complex?", 	NUMBERQ,   1);
    env->defPrim("cons",    	CONS,      2);
    env->defPrim("cos",     	COS,       1);
    env->defPrim("current-input-port", CURRENTINPUTPORT, 0);
    env->defPrim("current-output-port", CURRENTOUTPUTPORT, 0);
    env->defPrim("display",        DISPLAY,   1, 2);
    env->defPrim("eof-object?",    EOFOBJECTQ, 1);
    env->defPrim("eq?",     	EQQ,       2);
    env->defPrim("equal?",  	EQUALQ,    2);
    env->defPrim("eqv?",    	EQVQ,      2);
    env->defPrim("eval",           EVAL,      1, 2);
    env->defPrim("even?",          EVENQ,     1);
    env->defPrim("exact?",         INTEGERQ,  1);
    env->defPrim("exp",     	EXP,       1);
    env->defPrim("expt",    	EXPT,      2);
    env->defPrim("force",          FORCE,     1);
    env->defPrim("for-each",       FOREACH,   1, n);
    env->defPrim("gcd",            GCD,       0, n);
    env->defPrim("inexact?",       INEXACTQ,  1);
    env->defPrim("input-port?",    INPUTPORTQ, 1);
    env->defPrim("integer->char",  INTEGERTOCHAR,      1);
    env->defPrim("integer?",       INTEGERQ,  1);
    env->defPrim("lcm",            LCM,       0, n);
    env->defPrim("length",  	LENGTH,    1);
    env->defPrim("list",    	LIST,      0, n);
    env->defPrim("list->string", 	LISTTOSTRING, 1);
    env->defPrim("list->vector",   LISTTOVECTOR,      1);
    env->defPrim("list-ref", 	LISTREF,   2);
    env->defPrim("list-tail", 	LISTTAIL,  2);
    env->defPrim("list?",          LISTQ,     1);
    env->defPrim("load",           LOAD,      1);
    env->defPrim("log",     	LOG,       1);
    env->defPrim("macro-expand",   MACROEXPAND,1);
    env->defPrim("make-string", 	MAKESTRING,1, 2);
    env->defPrim("make-vector",    MAKEVECTOR,1, 2);
    env->defPrim("map",            MAP,       1, n);
    env->defPrim("max",     	MAX,       1, n);
    env->defPrim("member",  	MEMBER,    2);
    env->defPrim("memq",    	MEMQ,      2);
    env->defPrim("memv",    	MEMV,      2);
    env->defPrim("min",     	MIN,       1, n);
    env->defPrim("modulo",         MODULO,    2);
    env->defPrim("negative?",      NEGATIVEQ, 1);
    env->defPrim("newline", 	NEWLINE,   0, 1);
    env->defPrim("not",     	NOT,       1);
    env->defPrim("null?",   	NULLQ,     1);
    env->defPrim("number->string", NUMBERTOSTRING,   1, 2);
    env->defPrim("number?", 	NUMBERQ,   1);
    env->defPrim("odd?",           ODDQ,      1);
    env->defPrim("open-input-file",OPENINPUTFILE, 1);
    env->defPrim("open-output-file", OPENOUTPUTFILE, 1);
    env->defPrim("output-port?",   OUTPUTPORTQ, 1);
    env->defPrim("pair?",   	PAIRQ,     1);
    env->defPrim("peek-char",      PEEKCHAR,  0, 1);
    env->defPrim("positive?",      POSITIVEQ, 1);
    env->defPrim("procedure?", 	PROCEDUREQ,1);
    env->defPrim("quotient",       QUOTIENT,  2);
    env->defPrim("rational?",      INTEGERQ, 1);
    env->defPrim("read",    	READ,      0, 1);
    env->defPrim("read-char",      READCHAR,  0, 1);
    env->defPrim("real?", 	        NUMBERQ,   1);
    env->defPrim("remainder",      REMAINDER, 2);
    env->defPrim("reverse", 	REVERSE,   1);
    env->defPrim("round",  	ROUND,     1);
    env->defPrim("set-car!",	SETCAR,    2);
    env->defPrim("set-cdr!",	SETCDR,    2);
    env->defPrim("sin",     	SIN,       1);
    env->defPrim("sqrt",    	SQRT,      1);
    env->defPrim("string", 	STRING,    0, n);
    env->defPrim("string->list", 	STRINGTOLIST, 1);
    env->defPrim("string->number", STRINGTONUMBER,   1, 2);
    env->defPrim("string->symbol", STRINGTOSYMBOL,   1);
    env->defPrim("string-append",  STRINGAPPEND, 0, n);
    env->defPrim("string-ci<=?",   STRINGCICMP+LE, 2);
    env->defPrim("string-ci<?" ,   STRINGCICMP+LT, 2);
    env->defPrim("string-ci=?" ,   STRINGCICMP+EQ, 2);
    env->defPrim("string-ci>=?",   STRINGCICMP+GE, 2);
    env->defPrim("string-ci>?" ,   STRINGCICMP+GT, 2);
    env->defPrim("string-length",  STRINGLENGTH,   1);
    env->defPrim("string-ref", 	STRINGREF, 2);
    env->defPrim("string-set!", 	STRINGSET, 3);
    env->defPrim("string<=?",      STRINGCMP+LE, 2);
    env->defPrim("string<?",       STRINGCMP+LT, 2);
    env->defPrim("string=?",       STRINGCMP+EQ, 2);
    env->defPrim("string>=?",      STRINGCMP+GE, 2);
    env->defPrim("string>?",       STRINGCMP+GT, 2);
    env->defPrim("string?", 	STRINGQ,   1);
    env->defPrim("substring", 	SUBSTRING, 3);
    env->defPrim("symbol->string", SYMBOLTOSTRING,   1);
    env->defPrim("symbol?", 	SYMBOLQ,   1);
    env->defPrim("tan",     	TAN,       1);
    env->defPrim("vector",    	VECTOR,    0, n);
    env->defPrim("vector->list",   VECTORTOLIST, 1);
    env->defPrim("vector-length",  VECTORLENGTH, 1);
    env->defPrim("vector-ref",     VECTORREF, 2);
    env->defPrim("vector-set!",    VECTORSET, 3);
    env->defPrim("vector?",    	VECTORQ,   1);
    env->defPrim("write",   	WRITE,     1, 2);
    env->defPrim("write-char",   	DISPLAY,   1, 2);
    env->defPrim("zero?",          ZEROQ,     1);

    ///////////// Extensions ////////////////

    env->defPrim("new",     	    NEW,       1);
    env->defPrim("class",   	    CLASS,     1);
    env->defPrim("method",  	    METHOD,    2, n);
    env->defPrim("exit",    	    EXIT,      0, 1);
    env->defPrim("error",    	    ERROR,     0, n);
    env->defPrim("time-call",          TIMECALL,  1, 2);
    env->defPrim("_list*",             LISTSTAR,  0, n);
    ;

    return env;
}

boost::any primitive::apply(interpreter* interp, boost::any args)
{
    return TRUE(); // FIXME
}
