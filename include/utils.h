#ifndef UTILS
#define UTILS

#include <string.h>
#include <stdio.h>
#include <zephyr/zephyr.h>

static inline void to_display_format(const uint8_t *src, size_t size, char *dst)
{
	size_t i;

	for (i = 0; i < size; i++)
	{
		sprintf(dst + 5 * i, "0x%02x,", src[i]);
	}
}

#endif //UTILS