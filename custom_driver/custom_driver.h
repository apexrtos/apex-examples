#ifndef custom_driver_h
#define custom_driver_h

/*
 * A minimal example of a custom driver.
 *
 * This driver can be initialised using a configuration directive such as:
 * driver custom_driver("test string")
 *
 * To test the driver functionality try something like:
 * cat /dev/custom
 */

void custom_driver_init(const char *);

#endif
