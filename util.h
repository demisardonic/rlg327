#ifndef UTIL_H
#define UTIL_H

/* change conversion to weight to 1 + hardness / div */
extern int util_hpw;

/* convert to base 62 single char 0-9 a-z A-Z */
char util_base62(int);

/* display a help page when -h is specified */
void util_help();

/* reverse order of bytes */
unsigned int util_reverse(unsigned int);

/* generate a random number in [0, n) */
int util_rnum(int from, int to);

/* convert hardness to weight */
int util_weight(int hardness);

#endif
