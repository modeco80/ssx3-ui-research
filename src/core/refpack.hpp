#pragma once

#include <cstdint>
#include <vector>

namespace core::refpack {

	// FIXME: UnpackSize(u8* ptr);

	/**
	 * Decompress a RefPack source.
	 *
	 * \param[in] compressed Compressed span of data.
	 * \return A decompressed buffer.
	 */
	std::vector<std::uint8_t> Decompress(std::uint8_t* compressed);

} // namespace core::refpack