#ifndef __BIT_READER_HPP__
#define __BIT_READER_HPP__

#include "bit.hpp"
#include <istream>

using std::istream;

class bit_reader {
public:
	bit_reader (istream &_in) : in(_in){
		read = 0;
	}

	bit read_bit () {
		if (read == 8) {
			char next;
			in >> next;
			unpack(next, buffer);
			read = 0;
		}

		read++;
		return buffer[read - 1];
	}

	char read_char () {
		bit array[8];
		for (int i = 0; i < 8; i++) {
			array[i] = read_bit();
		}

		return pack(array);
	}

private:
	bit buffer[8];
	int read;
	istream &in;
};

#endif