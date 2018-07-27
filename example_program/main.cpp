#include <iostream>

#include "example.h"
#include "lib2/lib2.h"
#include <custom_driver/ioctl.h>
#include <fcntl.h>
#include <lib1/lib1.h>

int main()
{
	std::cout << "Example program" << std::endl;
	lib1_function();
	lib2_function();
	example_c_function();

	int fd = open("/dev/custom", O_RDWR);
	int arg = 42;
	std::cout << "Invoking custom driver ioctl with arg " << arg << std::endl;
	ioctl(fd, CUSTOM_DRIVER_IOC_TEST, &arg);

	return 0;
}
