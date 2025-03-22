
#pragma once
#include <core/types.hpp>
#include <span>
#include <string_view>

namespace core {

	struct MmapFile {
		struct MappedView {
			MappedView(const MmapFile&) = delete;

			~MappedView();

			std::span<u8> GetView() const {
				return { pMappedView, mappedViewSize };
			}

		   private:
			friend MmapFile;

			MappedView(u8* pMappedView, usize mappedViewSize)
				: pMappedView(pMappedView), mappedViewSize(mappedViewSize) {
			}

			u8* pMappedView;
			usize mappedViewSize;
		};

		MmapFile(const MmapFile&) = delete;
		MmapFile(MmapFile&&) = delete;
        ~MmapFile();

		static MmapFile Open(const std::string_view path);

		/// Maps a view of the file.
		MappedView MapView(
		usize length,
		usize offset = 0);

		MappedView MapWholeView();

	   protected:
		MmapFile(int fd);

	   private:
		int fd;
        usize wholeFileSize; // Cached
	};

} // namespace core