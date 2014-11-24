#include <iostream>
#include "boo.h"
#include "boo_types.h"
#include "base_library.h"

using namespace boo;

void test_syntax()
{
	input_port* in = new input_port(std::cin);
	output_port* out = new output_port(std::cout);
	continuation_write* cc = new continuation_write(out, true);

	while (true) 
	{
		std::cout << "syntax> " << std::flush;
		cc->apply(in->read());
		std::cout << std::endl;
	}
}

int main()
{
	test_syntax();
}
