#ifndef __BIT_HPP__
#define __BIT_HPP__

enum bit {off = 0, on = 1};

// Packs the first 8 elements of an array of bits into a char.
// Precondition: array needs to have size at least 8.
char pack (bit *array);

// Unpacks a char into an array of bits.
// Precondition: array needs to have size at least 8. 
void unpack (char c, bit *array);

#endif