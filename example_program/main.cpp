#include <iostream>

#include "example.h"
#include "lib2/lib2.h"
#include <lib1/lib1.h>

int main()
{
	std::cout << "Example program" << std::endl;
	lib1_function();
	lib2_function();
	example_c_function();
	return 0;
}
