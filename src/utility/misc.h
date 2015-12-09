#ifndef INCLUDE_MISC_H
#define INCLUDE_MISC_H

#define UNUSED(expr) (void)(expr)

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define in_range(x, a, b) (((x) >= (a)) && ((x) < (b)))

#endif
