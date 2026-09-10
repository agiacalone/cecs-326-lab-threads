/*
 * example_serial.c: a worked example. Read it, build it, run it.
 *
 *     make example
 *     ./example
 *
 * It fetches exactly one leaf from one scriptorium and prints it. That is
 * all. It is here so that Phase 1 starts from a shape you have already seen
 * working, instead of from an empty file.
 *
 * Notice four things, because you need all four in fetch.c:
 *
 *   1. mirror_open() is called once, first, and its return value is checked.
 *   2. The buffer is exactly MIRROR_LEAF bytes. Not more, not fewer.
 *   3. mirror_read()'s return value is checked. Every C function that can
 *      fail will eventually fail, and the ones you did not check are the ones
 *      that cost you an evening.
 *   4. The leaf is NOT a C string. It has no terminating '\0', so printf with
 *      %s would run off the end of it. fwrite takes an explicit length.
 */
#include "mirror.h"

#include <stdio.h>

int main(void)
{
    int map_size = mirror_open();
    if (map_size < 0) {
        fprintf(stderr, "example: the scriptoria would not open\n");
        return 1;
    }
    printf("the assembled map is %d bytes\n", map_size);
    printf("scriptorium 0 holds %d leaves\n\n", mirror_leaves(0));

    char leaf[MIRROR_LEAF];
    ssize_t got = mirror_read(0, 0, leaf);
    if (got != MIRROR_LEAF) {
        fprintf(stderr, "example: mirror_read returned %zd\n", got);
        return 1;
    }

    printf("leaf 0 of scriptorium 0 belongs at byte %lld:\n\n",
           (long long)mirror_offset(0, 0));
    fwrite(leaf, 1, (size_t)got, stdout);

    return 0;
}
