#include <core/refpack.hpp>

namespace core::refpack {

	// TODO: refpack compression

	std::vector<std::uint8_t> Decompress(std::uint8_t* compressed) {
		const std::uint8_t* in = compressed;

		// Command variables
		std::uint8_t first;
		std::uint8_t second;
		std::uint8_t third;
		std::uint8_t fourth;

		// output buffer
		std::vector<std::uint8_t> out;

		std::uint32_t proc_len;
		std::uint32_t ref_run;
		std::uint8_t* ref_ptr;

		// perform null & 0 size check
		if(!in)
			return {};

		std::uint16_t signature = ((in[0] << 8) | in[1]);
		in += sizeof(std::uint16_t);

		// skip uint24 compressed size field
		if(signature & 0x0100)
			in += 3;

		// read the uint24 decompressed size
		std::uint32_t decompressed_size = ((in[0] << 16) | (in[1] << 8) | in[2]);
		in += 3;

		// then resize output buffer to decompressed size
		// and retrive pointer to that data

		out.resize(decompressed_size);
		std::uint8_t* outptr = out.data();

		while(true) {
			// Retrive the first command byte
			first = *in++;

			if(!(first & 0x80)) {
				// 2-byte command: 0DDRRRPP DDDDDDDD
				second = *in++;

				proc_len = first & 0x03;

				for(std::uint32_t i = 0; i < proc_len; i++)
					*outptr++ = *in++;

				ref_ptr = outptr - ((first & 0x60) << 3) - second - 1;
				ref_run = ((first >> 2) & 0x07) + 3;

				for(std::uint32_t i = 0; i < ref_run; ++i)
					*outptr++ = *ref_ptr++;

			} else if(!(first & 0x40)) {
				// 3-byte command: 10RRRRRR PPDDDDDD DDDDDDDD
				second = *in++;
				third = *in++;

				proc_len = second >> 6;

				for(std::uint32_t i = 0; i < proc_len; ++i)
					*outptr++ = *in++;

				ref_ptr = outptr - ((second & 0x3f) << 8) - third - 1;
				ref_run = (first & 0x3f) + 4;

				for(std::uint32_t i = 0; i < ref_run; ++i)
					*outptr++ = *ref_ptr++;

			} else if(!(first & 0x20)) {
				// 4-byte command: 110DRRPP DDDDDDDD DDDDDDDD RRRRRRRR
				second = *in++;
				third = *in++;
				fourth = *in++;

				proc_len = first & 0x03;

				for(std::uint32_t i = 0; i < proc_len; ++i)
					*outptr++ = *in++;

				ref_ptr = outptr - ((first & 0x10) << 12) - (second << 8) - third - 1;
				ref_run = ((first & 0x0c) << 6) + fourth + 5;

				for(std::uint32_t i = 0; i < ref_run; ++i)
					*outptr++ = *ref_ptr++;
			} else {
				// 1-byte command: 111PPPPP

				proc_len = (first & 0x1f) * 4 + 4;

				if(proc_len <= 0x70) {
					// no stop flag

					for(std::uint32_t i = 0; i < proc_len; ++i)
						*outptr++ = *in++;

				} else {
					// has a stop flag
					proc_len = first & 0x3;

					for(std::uint32_t i = 0; i < proc_len; ++i)
						*outptr++ = *in++;

					break;
				}
			}
		}

		return out;
	}

} // namespace core::refpack