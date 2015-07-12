#ifndef INCLUDE_MISC_H
#define INCLUDE_MISC_H

#define UNUSED(expr) (void)(expr)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define in_range(x, a, b) (((x) >= (a)) && ((x) < (b)))

#endif
