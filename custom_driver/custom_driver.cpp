#include "init.h"

#include "ioctl.h"
#include <assert.h>
#include <debug.h>
#include <device.h>
#include <errno.h>
#include <fs/util.h>
#include <string.h>
#include <sys/param.h>

static const char *data;
static size_t data_len;

/*
 * Copy test data to buffer
 */
static ssize_t
read(void *buf, size_t len, off_t offset)
{
	if (offset >= data_len)
		return 0;
	const size_t r = MIN(data_len - offset, len);
	memcpy(buf, data + offset, r);
	return r;
}

static ssize_t
read_iov(file *file, const iovec *iov, size_t count, off_t offset)
{
	return for_each_iov(iov, count, offset,
	    [](std::span<std::byte> buf, off_t offset) {
		return read(buf.data(), buf.size(), offset);
	});
}

/*
 * Ignore data written to this driver
 */
static ssize_t
write_iov(file *file, const iovec *iov, size_t count, off_t offset)
{
	return for_each_iov(iov, count, offset,
	    [](std::span<std::byte> buf, off_t offset) {
		return buf.size();
	});
}

/*
 * Example ioctl
 */
int custom_driver_ioctl(file *file, u_long cmd, void *arg)
{
	switch (cmd) {
	case CUSTOM_DRIVER_IOC_TEST:
		info("custom_driver_ioctl test %d\n", *(int *)arg);
		return 0;
	default:
		return DERR(-EINVAL);
	};
}

/*
 * Initialize
 */
void
custom_driver_init(const char *str)
{
	static devio io = {
		.read = read_iov,
		.write = write_iov,
		.ioctl = custom_driver_ioctl,
	};
	data = str;
	data_len = strlen(str);

	/* Create device object */
	device *d = device_create(&io, "custom", DF_CHR, NULL);
	assert(d);
}
