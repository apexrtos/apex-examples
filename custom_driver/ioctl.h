#ifndef custom_driver_ioctl_h
#define custom_driver_ioctl_h

#include <sys/ioctl.h>

/*
 * Some (hopefully unique) byte to catch bugs if an ioctl is invoked on an
 * inappropriate file.
 */
#define CUSTOM_DRIVER_IOCTL_MAGIC 'C'

/*
 * A silly ioctl which writes the value of it's argument to the system log
 */
#define CUSTOM_DRIVER_IOC_TEST _IOW(CUSTOM_DRIVER_IOCTL_MAGIC, 0, int)

#endif
