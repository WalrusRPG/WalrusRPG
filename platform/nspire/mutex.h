/* Recap on swp:
 * swp rx, ry, [rz]
 * In one instruction:
 * 1) Stores the value in ry into location pointed by rz.
 * 2) Loads the value in the location of rz into rx.
 * By doing so, in one instruction one can attempt to lock
 * a word, and discover whether it was already locked.
 */

inline void spin_lock(unsigned *s)
{
    unsigned tmp = 1, tmp2;
    __asm__ __volatile__(
        "1: \n"
        "swp %0, %1, [%2] \n"
        "teq %0, %3 \n"
        "bne 1b \n"
        : "=&r"(tmp2)
        : "r"(tmp), "r"(s), "r"(0)
        : "cc", "memory");
}

int mutex_lock(unsigned *s)
{
    unsigned tmp = 1, tmp2;
    __asm__ __volatile__("swp %0, %1, [%2] \n"
                         : "=&r"(tmp2)
                         : "r"(tmp), "r"(s)
                         : "cc", "memory");

    return tmp2;
}

inline void mutex_unlock(unsigned *s)
{
    *s = 0;
}
