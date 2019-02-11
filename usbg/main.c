#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb/gadget/ioctl.h>

void
usage(void)
{
	const char* usage =
	    "Usage: %s [options]\n"
	    "Options:\n"
	    "  set_device_descriptor udc bDeviceClass bDeviceSubClass bDeviceProtocol idVendor idProduct bcdDevice Manufacturer Product SerialNumber\n"
	    "  add_configuration udc ident Configuration bmAttributes bMaxPower\n"
	    "  add_function udc configs function data\n"
	    "  start udc\n"
	    "  stop udc\n"
	    "Examples:\n"
	    "  Composite device\n"
	    "    set_device_descriptor udc0 0 0 0 0x1234 0x5678 0x0001 \"Dummy Inc.\" \"Dummy Product\" \"1234\"\n"
	    "  Composite device with interface association descriptors\n"
	    "    set_device_descriptor udc0 0xef 2 1 0x1234 0x5678 0x0001 \"Dummy Inc.\" \"Dummy Product\" \"1234\"\n"
	    "  Add configuration (note, bMaxPower in 2mA units)\n"
	    "    add_configuration udc0 dummy \"Dummy Configuration\" 250\n";
	fprintf(stderr, usage, program_invocation_name);
	exit(EXIT_FAILURE);
}

void
error(int status, int errn, const char *fmt, ...)
{
	va_list args;

	fflush(stdout);
	fprintf(stderr, "%s: ", program_invocation_name);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (errn)
		fprintf(stderr, ": %s\n", strerror(errn));
	else
		fprintf(stderr, "\n");
	fflush(stderr);
	if (status)
		exit(status);
}

unsigned long
parseul(const char* s)
{
	unsigned long ret;
	errno = 0;
	char* p;
	ret = strtoul(s, &p, 0);
	if (errno != 0)
		error(EXIT_FAILURE, errno, "%s", s);
	if (s == p)
		error(EXIT_FAILURE, EINVAL, "%s", s);
	if (*p)
		error(EXIT_FAILURE, EINVAL, "%s", s);
	return ret;
}

int
main(int argc, const char *argv[])
{
	if (argc < 2)
		usage();

	int fd;
	if ((fd = open("/dev/usbgadget", O_RDWR)) < 0)
		error(EXIT_FAILURE, errno, "failed to open /dev/usbgadget");

	if (!strcmp(argv[1], "set_device_descriptor")) {
		if (argc != 12)
			usage();
		struct usbg_ioctl_set_device_descriptor s;
		s.udc = argv[2];
		s.bDeviceClass = parseul(argv[3]);
		s.bDeviceSubClass = parseul(argv[4]);
		s.bDeviceProtocol = parseul(argv[5]);
		s.idVendor = parseul(argv[6]);
		s.idProduct = parseul(argv[7]);
		s.bcdDevice = parseul(argv[8]);
		s.Manufacturer = argv[9];
		s.Product = argv[10];
		s.SerialNumber = argv[11];
		if (ioctl(fd, USBG_IOC_SET_DEVICE_DESCRIPTOR, &s) < 0)
			error(EXIT_FAILURE, errno, "USBG_IOC_SET_DEVICE_DESCRIPTOR failed");
	} else if (!strcmp(argv[1], "add_configuration")) {
		if (argc != 7)
			usage();
		struct usbg_ioctl_add_configuration s;
		s.udc = argv[2];
		s.name = argv[3];
		s.Configuration = argv[4];
		s.bmAttributes = parseul(argv[5]);
		s.bMaxPower = parseul(argv[6]);
		if (ioctl(fd, USBG_IOC_ADD_CONFIGURATION, &s) < 0)
			error(EXIT_FAILURE, errno, "USBG_IOC_ADD_CONFIGURATION failed");
	} else if (!strcmp(argv[1], "add_function")) {
		if (argc != 5 && argc != 6)
			usage();
		struct usbg_ioctl_add_function s;
		s.udc = argv[2];
		s.configs = argv[3];
		s.function = argv[4];
		s.data = argc == 6 ? argv[5] : "";
		if (ioctl(fd, USBG_IOC_ADD_FUNCTION, &s) < 0)
			error(EXIT_FAILURE, errno, "USBG_IOC_ADD_FUNCTION failed");
	} else if (!strcmp(argv[1], "start")) {
		if (argc != 3)
			usage();
		if (ioctl(fd, USBG_IOC_START, argv[2]) < 0)
			error(EXIT_FAILURE, errno, "USBG_IOC_START failed");
	} else if (!strcmp(argv[1], "stop")) {
		if (argc != 3)
			usage();
		if (ioctl(fd, USBG_IOC_STOP, argv[2]) < 0)
			error(EXIT_FAILURE, errno, "USBG_IOC_START failed");
	} else
		usage();

	return EXIT_SUCCESS;
}
