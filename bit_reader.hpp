#ifndef __BIT_READER_HPP__
#define __BIT_READER_HPP__

#include "bit.hpp"
#include <istream>
#include <cstring>

using std::istream;

class bit_reader {
public:
	bit_reader (istream &_in) : in(_in){
		remaining = 0;
		eofbit = false;
	}

	bit read_bit () {
		if (remaining == 0) {
			refill();
		}

		if (remaining == 0) {
			// We couldn't refill the buffer, so we have reached the end of file.
			eofbit = true;
			return bit(-1);
		}

		bit ret = buffer[8 - remaining];
		remaining--;
		return ret;
	}

	char read_char () {
		bit array[8];
		for (int i = 0; i < 8; i++) {
			array[i] = read_bit();
		}

		return pack(array);
	}

	int read_int () {
		int ret = 0;
		for (int k = 0; k < 4; k++) {
			char c = read_char();
			memcpy((char *)(&ret) + k, &c, 1);
		}

		return ret;
	}

	bit peek() {
		if (remaining > 0){
			return buffer[8 - remaining];
		}

		refill();

		if (remaining == 0) {
			eofbit = true;
			return bit(-1);
		}

		return buffer[0];
	}

	bool eof(){
		return eofbit;
	}

private:
	void refill () {
		char next;
		in >> next;

		if (!in.eof()) {
			unpack(next, buffer);
			remaining = 8;
		}
	}

	bit buffer[8];
	int remaining;
	istream &in;
	bool eofbit;
};

#endif