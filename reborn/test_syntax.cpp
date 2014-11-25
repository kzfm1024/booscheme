#include <assert.h>
#include <iostream>
#include <sstream>
#include "boo.h"
#include "boo_objects.h"
#include "base_library.h"

using namespace boo;

void test_syntax()
{
	environment* env = new environment(NIL(), NIL(), 0);
	base_library::install(env);

	std::istringstream stream(
		"(define-syntax and"
		"(syntax-rules ()"
		"((and) #t)"
		"((and test) test)"
		"((and test1 test2 ...)"
		"(if test1 (and test2 ...) #f))))"
							  
		"(and 3 5 4)"
		);
	
	input_port* in = new input_port(stream);

	object* x = in->read();
	PDEBUG("define-syntax", x);
		
	syntax* synx = dynamic_cast<syntax*>(eval(x, env));
	assert(synx);
	PDEBUG("syntax", synx);
		
	x = in->read();
	PDEBUG("expr", x);
	x = synx->expand(x);
	PDEBUG("expand", x);
}

void test_syntax2()
{
	environment* env = new environment(NIL(), NIL(), 0);
	base_library::install(env);

	std::istringstream stream(
		"(define-syntax and"
		"(syntax-rules ()"
		"((and) #t)"
		"((and test) test)"
		"((and test1 test2 ...)"
		"(if test1 (and test2 ...) #f))))"

		"(and 3 5 4)"
		);
	
	input_port* in = new input_port(stream);
	output_port* out = new output_port(std::cout);

	object* x;
	do
	{
		std::cout << "eval> " << std::flush;
		x = eval(in->read(), env);
		write(x, out, true);
		std::cout << std::endl;
	} 
	while (!is_eof(x));
}

int main()
{
	//test_syntax();
	test_syntax2();
}
