#ifndef INCLUDE_MISC_H
#define INCLUDE_MISC_H

#include <cstdint>

#define UNUSED(expr) (void)(expr)

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define in_range(x, a, b) (((x) >= (a)) && ((x) < (b)))

/**
 * Read big endian integer value in binary data array. Depends on the size
 * of the given type, so using standardized types like stdint's is highly
 * suggested.
 *
 * Also doesn't check for being in bounds of the array.
 */
  template <typename T> T read_big_endian_value(const void *ptr)
{
    T result = 0;
    uint8_t *data = (uint8_t *) ptr;
    // No, memcpy isn't working here because endianness issues.
    for (unsigned i = 0; i < sizeof(T); i++)
        // Writing each byte of value in order
        result |= data[i] << (8 * (sizeof(T) - 1 - i));
    return result;
}
#endif
