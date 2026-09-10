/*
 * mirror.h: the scriptoria of CECS 326 Lab 1: The Torn Map
 *
 * You do not need to read the implementation. You do need to read this file.
 * Every function you are given lives here, and the comments say exactly what
 * each one promises.
 *
 * Link against the object file for your machine, e.g.
 *
 *     gcc -Wall -Werror -pthread fetch.c lib/mirror_x86_64.o -o fetch
 *
 * The Makefile picks the right one for you.
 */
#ifndef MIRROR_H
#define MIRROR_H

#include <stddef.h>
#include <sys/types.h>

/* How many scriptoria hold pieces of the map. One runner each. */
#define MIRROR_COUNT 7

/* Every leaf is exactly this many bytes. No short reads, no partial leaves. */
#define MIRROR_LEAF 512

/*
 * Wake the scriptoria and return the total size of the assembled map in
 * bytes. Call this exactly once, before anything else, from the main thread.
 * Returns a negative value on failure.
 */
int mirror_open(void);

/*
 * How many leaves scriptorium `scriptorium` holds (0 <= scriptorium <
 * MIRROR_COUNT). Returns -1 if the scriptorium does not exist.
 */
int mirror_leaves(int scriptorium);

/*
 * Where leaf `leaf` of `scriptorium` belongs in the assembled map.
 *
 * The map was torn and scattered, so this is NOT simply
 * scriptorium * leaves * MIRROR_LEAF. Use this function; do not compute the
 * offset yourself. Returns -1 for a scriptorium or leaf that does not exist.
 */
off_t mirror_offset(int scriptorium, int leaf);

/*
 * Copy one leaf into `buf`, which must have room for MIRROR_LEAF bytes.
 * Returns the number of bytes written (always MIRROR_LEAF on success), or -1.
 *
 * This is SLOW, on purpose. A scriptorium takes several milliseconds to find
 * a leaf and copy it out, and they do not all take the same time. That is the
 * whole reason this lab wants more than one runner.
 *
 * It is safe to call from many threads at once.
 */
ssize_t mirror_read(int scriptorium, int leaf, char *buf);

/*
 * The largest number of mirror_read() calls that were ever in flight at the
 * same time. One runner start to finish gives 1. Seven runners genuinely
 * overlapping gives 7.
 *
 * The grader reads this. So can you: it is the quickest way to find out
 * whether your threads are actually running at the same time, or whether you
 * accidentally joined each one right after you created it.
 */
int mirror_max_concurrency(void);

/*
 * The checksum of the correctly assembled map. Compare it against
 * fnv1a() over your own buffer to check your work.
 *
 * Knowing the checksum does not tell you the bytes. You still have to fetch
 * every leaf and put it in the right place.
 */
unsigned long long mirror_expected_checksum(void);

/*
 * FNV-1a, 64-bit. The same function the scriptoria use, so you can compare
 * your assembled map against mirror_expected_checksum().
 */
static inline unsigned long long fnv1a(const char *data, size_t len)
{
    unsigned long long h = 1469598103934665603ULL;
    for (size_t i = 0; i < len; i++) {
        h ^= (unsigned char)data[i];
        h *= 1099511628211ULL;
    }
    return h;
}

#endif /* MIRROR_H */
