#ifndef __BIT_WRITER_HPP__
#define __BIT_WRITER_HPP__

#include "bit.hpp"
#include <ostream>

using std::ostream;

class bit_writer {
public:
	bit_writer (ostream &_out) : out(_out) {
		count = 0;
		clear();
	}

	~bit_writer () {
		flush();
	}

	void write_bit (bit b) {
		// count < 8
		buffer[count] = b;
		count++;

		if (count == 8) {
			flush();
		}
	}

	void write_char (char c) {
		bit unpacked[8];
		unpack(c, unpacked);

		for (int i = 0; i < 8; i++) {
			write_bit(unpacked[i]);
		}
	}

	void flush () {
		if (count > 0) {
			out << pack(buffer);
		}

		clear();
		count = 0;	
	}

private:
	void clear () {
		for (int i = 0; i < 8; i++) {
			buffer[i] = bit(0);
		}
	}

	bit buffer[8];
	int count;
	ostream &out;
};

#endif
