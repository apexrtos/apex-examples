#ifndef custom_driver_init_h
#define custom_driver_init_h

/*
 * A minimal example of a custom driver.
 *
 * This driver can be initialised using a configuration directive such as:
 * driver custom_driver("test string")
 *
 * To test the driver functionality try something like:
 * cat /dev/custom
 */

#ifdef __cplusplus
extern "C" {
#endif

void custom_driver_init(const char *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
