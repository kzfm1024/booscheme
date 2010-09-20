//
// booscheme.h
//

#ifndef _BOOSCHEME_H
#define _BOOSCHEME_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#if defined(_ENABLE_BOODEBUG_)
#define BOODEBUG(x) x
boost::any p(boost::any x);
boost::any p(const std::string& msg, boost::any x);
#else
#define BOODEBUG(x)
#endif

class symbol;
class input_port;
class output_port;
class procedure;
class closure;
class macro;
class primitive;
class environment;
class end_of_file;
class misc;
class interpreter;

typedef boost::shared_ptr<boost::any>                         Object;
typedef boost::shared_ptr<bool>                               Boolean;
typedef boost::shared_ptr<std::pair<boost::any, boost::any> > Pair;
typedef boost::shared_ptr<symbol>                             Symbol;
typedef boost::shared_ptr<long int>                           Number; // FIXME
typedef boost::shared_ptr<char>                               Char; // FIXME
typedef boost::shared_ptr<std::string>                        String;
typedef boost::shared_ptr<std::vector<boost::any> >           Vector;
typedef boost::shared_ptr<input_port>                         InputPort;
typedef boost::shared_ptr<output_port>                        OutputPort;
typedef boost::shared_ptr<procedure>                          Procedure;
typedef boost::shared_ptr<closure>                            Closure;
typedef boost::shared_ptr<macro>                              Macro;
typedef boost::shared_ptr<primitive>                          Primitive;
typedef boost::shared_ptr<environment>                        Environment;
typedef boost::shared_ptr<end_of_file>                        EndOfFile;
typedef boost::shared_ptr<misc>                               Misc;

class end_of_file
{
};

class input_port
{
public:
    input_port(std::istream& is)
        : isPushedToken(false), isPushedChar(false),
          pushedToken(), pushedChar(-1), in(is), buff() {}
    ~input_port() {}

    boost::any readChar();
    boost::any peekChar();
    boost::any read();
    
private:
    static end_of_file eof;

    int peekCh();
    int pushChar(int ch);
    int popChar();

    boost::any readTail();
    boost::any nextToken();

    bool          isPushedToken;
    bool          isPushedChar;
    boost::any    pushedToken;
    int           pushedChar;
    std::istream& in;
    std::string   buff;
};

class output_port
{
public:
    output_port(std::ostream& os) : out(os) {}
    ~output_port() {}
    
    void write(const std::string& s);
    void newline();
    void flush();

private:
    std::ostream& out;
};

class misc
{
public:
    misc(const char* s) : str(s) {}
    misc(const std::string& s) : str(s) {}
    ~misc() {}
    
    const std::string str;
};


class symbol
{
public:
    symbol(const std::string& n) : name(n) {}
    ~symbol() {} 

    static Symbol make(const std::string& n); // FIXME: change to get()?
    std::string name;

private:
    static std::map<std::string, Symbol> table;
};

class environment
{
public:
    environment();
    environment(boost::any vars, boost::any vals, Environment parent);
    ~environment() {}

    boost::any lookup(Symbol sym);
    boost::any define(boost::any var, boost::any val);
    boost::any set(boost::any var, boost::any val);
    Environment defPrim(const std::string& n, int id, int minArgs);
    Environment defPrim(const std::string& n, int id, int minArgs, int maxArgs);
    bool numberArgsOK(boost::any vars, boost::any vals);

private:
    boost::any vars;
    boost::any vals;
    Environment parent;
};

class procedure
{
public:
    procedure() : name("anonymous procedure") {}
    virtual ~procedure() {}

    std::string name;
};

class closure : public procedure
{
public:
    closure(boost::any p, boost::any b, Environment e);
    virtual ~closure() {}

public:
    boost::any parms;
    boost::any body;
    Environment env;
};

class macro : public closure
{
};

class primitive : public procedure
{
public:
    primitive(int id, int minArgs, int maxArgs);
    virtual ~primitive() {}

    static Environment installPrimitives(Environment env);
    boost::any apply(interpreter* interp, boost::any args);

private:
    int id;
    int minArgs;
    int maxArgs;
};

class interpreter
{
public:
    interpreter();
    ~interpreter() {}

    void repl(); // read-eval-print loop
    boost::any load(const std::string& path);
    boost::any load(InputPort in);
    boost::any eval(boost::any x, Environment env);
    boost::any eval(boost::any x);
    
    InputPort  inputPort()  { return input;  }
    OutputPort outputPort() { return output; }

private:
    boost::any evalList(boost::any list, Environment env);
    boost::any reduceCond(boost::any clauses, Environment env);

    InputPort input;
    OutputPort output;
    Environment globalEnvironment;
};

boost::any error(const std::string& message);
boost::any warn(const std::string& message);
boost::any EMPTY();
boost::any TRUE();
boost::any FALSE();
bool truth(boost::any x);
long int num(boost::any x); // FIXME
char chr(boost::any x); // FIXME
String str(boost::any x);
Symbol sym(boost::any x);
Vector vec(boost::any x);
Procedure proc(boost::any x);
InputPort inPort(boost::any x, interpreter* interp);
OutputPort outPort(boost::any x, interpreter* interp);

boost::any first(boost::any x);
boost::any rest(boost::any x);
boost::any setFirst(boost::any x, boost::any y);
boost::any setRest(boost::any x, boost::any y);
boost::any second(boost::any x);
boost::any third(boost::any x);
boost::any list(boost::any a, boost::any b);
boost::any list(boost::any a);
boost::any listStar(boost::any args);
Pair cons(boost::any a, boost::any b);
boost::any reverse(boost::any x);
boost::any eq(boost::any x, boost::any y);
boost::any eqv(boost::any x, boost::any y);
boost::any equal(boost::any x, boost::any y);
int length(boost::any x);
boost::any listToString(boost::any chars); // FIXME: return type
boost::any listToVector(boost::any objs);  // FIXME: return type
boost::any vectorToList(boost::any x);     // FIXME: return type
boost::any write(boost::any x, OutputPort port, bool quoted);
std::string stringify(boost::any x, bool quoted);
std::string stringify(boost::any x);

bool isPair(boost::any x);
bool isSymbol(boost::any x);
bool isSymbol(boost::any x, const std::string& s);
bool isProcedure(boost::any x);
bool isProcedure(boost::any x, const std::string& s);
bool isClosure(boost::any x);
bool isMacro(boost::any x);
bool isEOF(boost::any x);
bool isEMPTY(boost::any x);
bool isMisc(boost::any x, const char* s);

Number toNumber(const std::string& s); // FIXME

#endif // _BOOSCHEME_H
