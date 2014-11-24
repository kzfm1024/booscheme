#include <iostream>
#include "boo.h"
#include "boo_types.h"
#include "base_library.h"

using namespace boo;

void test_eval_cc()
{
	environment* env = new environment(NIL(), NIL(), 0);
	base_library::install_cc(env);

	input_port* in = new input_port(std::cin);
	output_port* out = new output_port(std::cout);
	continuation_write* cc = new continuation_write(out, true);

	while (true) 
	{
		std::cout << "eval_cc> " << std::flush;
		eval_cc(in->read(), env, cc);
		std::cout << std::endl;
	}
}

int main()
{
	test_eval_cc();
}
