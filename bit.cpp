#include "bit.hpp"

char pack (bit *array) {
	unsigned char ret = 0;

	for (int i = 0; i < 8; i++) {
		ret = ret << 1;
		if (array[i] == 1) {
			ret += 1;
		}
	}

	return (char)ret;
}

void unpack (char c, bit *array) {
	unsigned char uc = (unsigned char) c;

	for (int i = 0; i < 8; i++) {
		array[7 - i] = bit(uc % 2);
		uc = uc >> 1;
	}
}