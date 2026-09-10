/*
 * warmup.c: Phase 0. Prove your toolchain works.
 *
 * If you have never compiled a C program before, this is the whole ritual:
 *
 *     gcc -Wall -Werror warmup.c -o warmup
 *     ./warmup
 *
 * `gcc` is the compiler. `-Wall` turns on the warnings you want. `-Werror`
 * makes those warnings fatal, so you cannot ignore them. `-o warmup` names
 * the program it produces. Then you run it.
 *
 * There is one deliberate mistake below. The compiler will refuse to build
 * this file until you fix it. Read what it tells you: the error message names
 * the file, the line, and the problem. Learning to read that message is worth
 * more to you this semester than any single lecture.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *party[] = { "barbarian", "wizard", "rogue" };
    int n = sizeof(party) / sizeof(party[0]);

    printf("The party assembles:\n");
    for (int i = 0; i < n; i++)
        printf("  %d. %s (%zu letters)\n", i + 1, party[i], strlen(party[i]));

    /* FIXME: this line does not compile. Work out why, then fix it.
     * Hint: what type is `party[0]`, and what type does `%d` expect? */
    printf("first member: %d\n", party[0]);

    printf("\nToolchain works. On to Phase 1.\n");
    return 0;
}
