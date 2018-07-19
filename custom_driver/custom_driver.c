#include "custom_driver.h"

#include <assert.h>
#include <device.h>
#include <fs/file.h>
#include <fs/util.h>
#include <string.h>
#include <sys/param.h>

static const char *data;
static size_t data_len;

/*
 * Copy test data to buffer
 */
static ssize_t
read(struct file *file, void *buf, size_t len)
{
	if (file->f_offset >= data_len)
		return 0;
	const size_t r = MIN(data_len - file->f_offset, len);
	memcpy(buf, data + file->f_offset, r);
	file->f_offset += r;
	return r;
}

static ssize_t
read_iov(struct file *file, const struct iovec *iov, size_t count)
{
	return for_each_iov(file, iov, count, read);
}

/*
 * Ignore data written to this driver
 */
static ssize_t
write(struct file *file, void *buf, size_t len)
{
	return len;
}

static ssize_t
write_iov(struct file *file, const struct iovec *iov, size_t count)
{
	return for_each_iov(file, iov, count, write);
}

/*
 * Initialize
 */
void
custom_driver_init(const char *str)
{
	static struct devio io = {
		.read = read_iov,
		.write = write_iov,
	};
	data = str;
	data_len = strlen(str);

	/* Create device object */
	struct device *d = device_create(&io, "custom", DF_CHR, NULL);
	assert(d);
}
