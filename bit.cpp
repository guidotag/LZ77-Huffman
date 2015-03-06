#include "bit.hpp"

char pack (bit *array) {
	unsigned char ret = 0;

	for (int i = 0; i < 8; i++) {
		if (array[i] == 1) {
			ret += 1;
		}
		ret = ret << 1;
	}

	return (char)ret;
}

void unpack (char c, bit *array) {
	unsigned char uc = (unsigned char) c;

	for (int i = 0; i < 8; i++) {
		array[7 - i] = (uc % 2 == 0) ? bit(1) : bit(0);
		uc = uc >> 1;
	}
}