//
// booscheme.h
//

#ifndef _BOOSCHEME_H
#define _BOOSCHEME_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#if defined(_BOOSCHEME_DEBUG_)
#include <iostream> // debugging purpose
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

class empty
{
};

class end_of_file
{
};

class symbol
{
public:
    symbol(const char* s) : str(s) {}
    symbol(const std::string& s) : str(s) {}
    ~symbol() { DEBUG(std::cout << "~symbol " << str << std::endl); }
    
    const std::string str;

private:
    // static std::map<std::string, boost::any> symtab; // NOT_YET
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

class environment;

typedef boost::shared_ptr<empty>  Empty;
typedef boost::shared_ptr<int> Number; // FIXME
typedef boost::shared_ptr<std::string> String;
typedef boost::shared_ptr<char> Char; // FIXME
typedef boost::shared_ptr<symbol> Symbol;
typedef boost::shared_ptr<std::pair<boost::any, boost::any> > Pair;
typedef boost::shared_ptr<std::vector<boost::any> > Vector;
typedef boost::shared_ptr<environment> Environment;
typedef boost::shared_ptr<input_port> InputPort;
typedef boost::shared_ptr<output_port> OutputPort;
typedef boost::shared_ptr<misc> Misc;

class environment
{
public:
    environment() : env(), parent() {}
    environment(boost::any vars, boost::any vals, Environment parent);
    ~environment() {}

    boost::any lookup(Symbol sym);
    boost::any define(boost::any var, boost::any val);
    boost::any set(boost::any var, boost::any val);
    bool numberArgsOK(boost::any vars, boost::any vals);

private:
    std::map<std::string, boost::any> env;
    Environment parent;
};

void error(const std::string& message);
void warn(const std::string& message);
Empty null();
bool truth(boost::any x);
int num(boost::any x);
char chr(boost::any x);
String str(boost::any x);

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
int length(boost::any x);
boost::any listToString(boost::any chars);
boost::any listToVector(boost::any objs);
boost::any vectorToList(boost::any x);
boost::any write(boost::any x, OutputPort port, bool quoted);
std::string stringify(boost::any x, bool quoted);
std::string stringify(boost::any x);
boost::any p(boost::any x);
boost::any p(const std::string& msg, boost::any x);

// bool isEmpty(boost::an x);
bool isEOF(boost::any x);
bool isSymbol(boost::any x, const char* s);
bool isMisc(boost::any x, const char* s);

#endif // _BOOSCHEME_H
