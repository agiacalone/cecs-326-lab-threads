/*
 * fetch.c: CECS 326 Lab 1: The Torn Map
 *
 * This is your file. Everything marked TODO is yours to write.
 *
 * Read the README first. Then read mirror.h, which describes every function
 * the scriptoria give you. Then come back here.
 *
 * Build and run:
 *     make
 *     ./fetch out.map
 */
#define _POSIX_C_SOURCE 200809L

#include "mirror.h"

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ------------------------------------------------------------------------
 * Shared state.
 *
 * Every runner writes into g_map, and every runner counts the leaves it has
 * recovered. One of those two things is safe to do without a lock and one is
 * not. Working out which is which is most of this assignment.
 * ------------------------------------------------------------------------ */

static char *g_map;             /* the assembled map */
static int   g_leaves_recovered; /* how many leaves have come home */

/*
 * PROVIDED. Do not change the body of this function.
 *
 * A real counter update is read, add, write. On a modern CPU those three
 * steps are over almost too fast to lose, which makes a race look like bad
 * luck instead of a bug. The yield and the short sleep in the middle stretch
 * the window out to something you can observe every time, the way a longer
 * critical section would in real code.
 *
 * They are not the bug. Take them out and the bug is still there; you just
 * stop being able to see it reliably. Your writeup will be asked about this.
 */
void record_leaf(void)
{
    int seen = g_leaves_recovered;              /* read  */
    struct timespec pause = { 0, 100000 };      /* 100 microseconds */
    sched_yield();                              /* give up the CPU ... */
    nanosleep(&pause, NULL);                    /* ... and stay gone a moment */
    g_leaves_recovered = seen + 1;              /* write */
}

/* ------------------------------------------------------------------------
 * Phase 2: one runner per scriptorium.
 * ------------------------------------------------------------------------ */

struct runner {
    int scriptorium;
    /* TODO (Phase 2): add anything else a runner needs to know. */
};

void *run_scriptorium(void *arg)
{
    struct runner *r = arg;
    (void)r;

    /*
     * TODO (Phase 2): fetch every leaf this scriptorium holds.
     *
     *   - mirror_leaves()  tells you how many there are
     *   - mirror_read()    fetches one into a local buffer
     *   - mirror_offset()  says where in g_map it belongs
     *   - record_leaf()    counts it
     *
     * The map was torn and scattered. A scriptorium's leaves are NOT
     * contiguous in the finished map. Use mirror_offset() for every leaf;
     * do not try to work the position out yourself.
     */

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <output-file>\n", argv[0]);
        return 1;
    }

    int map_size = mirror_open();
    if (map_size < 0) {
        fprintf(stderr, "fetch: the scriptoria would not open\n");
        return 1;
    }

    g_map = malloc((size_t)map_size);
    if (!g_map) {
        fprintf(stderr, "fetch: out of memory for a %d-byte map\n", map_size);
        return 1;
    }

    /*
     * TODO (Phase 1): fetch the whole map with no threads at all.
     * Loop over every scriptorium and every leaf. Get it correct first.
     *
     * TODO (Phase 2): replace that loop with MIRROR_COUNT threads, one per
     * scriptorium, using pthread_create() and pthread_join(). Create all of
     * them BEFORE you join any of them. Joining each one right after you
     * create it runs them one at a time, and mirror_max_concurrency() will
     * tell on you.
     *
     * TODO (Phase 3): make g_leaves_recovered come out right, every run.
     */

    /* Write the assembled map out. */
    FILE *out = fopen(argv[1], "wb");
    if (!out) {
        perror("fetch: fopen");
        free(g_map);
        return 1;
    }
    if (fwrite(g_map, 1, (size_t)map_size, out) != (size_t)map_size) {
        fprintf(stderr, "fetch: short write\n");
        fclose(out);
        free(g_map);
        return 1;
    }
    fclose(out);

    /*
     * The report. The grader reads these three lines, and so should you.
     * Do not change their format.
     */
    unsigned long long mine = fnv1a(g_map, (size_t)map_size);
    printf("leaves recovered : %d\n", g_leaves_recovered);
    printf("max concurrency  : %d\n", mirror_max_concurrency());
    printf("checksum         : %016llx %s\n", mine,
           mine == mirror_expected_checksum() ? "OK" : "MISMATCH");

    free(g_map);
    return 0;
}
