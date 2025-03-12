#ifndef COMMON_H
#define COMMON_H

#define HIGH_BITS(v) v >> 4
#define LOW_BITS(v) v & 0b00001111

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

#endif //COMMON_H
