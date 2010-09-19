//
// primitive.cpp
//

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

primitive::primitive(int id, int minArgs, int maxArgs)
    : id(id), minArgs(minArgs), maxArgs(maxArgs)
{
}

Environment
primitive::installPrimitives(Environment env)
{
    return env; // FIXME
}

boost::any primitive::apply(interpreter* interp, boost::any args)
{
    return TRUE(); // FIXME
}
