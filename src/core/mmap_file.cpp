
#include <fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <unistd.h>

#include <core/mmap_file.hpp>
#include <cstdio>
#include <system_error>

namespace core {

	MmapFile::MappedView::~MappedView() {
		if(pMappedView) {
			munmap(pMappedView, mappedViewSize);
			pMappedView = nullptr;
		}
		mappedViewSize = 0;
	}

	MmapFile MmapFile::Open(const std::string_view path) {
		if(auto fd = open(path.data(), O_RDONLY | O_CLOEXEC); fd != -1) {
			return MmapFile(fd);
		} else {
			// errno is mappable to system_category
			throw std::system_error { errno, std::generic_category() };
		}
	}

	MmapFile::MmapFile(int fd)
		: fd(fd) {
		// Cache the file's size.
		lseek64(fd, 0, SEEK_END);
		wholeFileSize = lseek64(fd, 0, SEEK_CUR);
		lseek64(fd, 0, SEEK_SET);
	}

	MmapFile::~MmapFile() {
		if(fd != -1) {
			close(fd);
			fd = -1;
		}
	}

	MmapFile::MappedView MmapFile::MapView(usize length, usize offset) {
		if(offset > wholeFileSize)
			throw std::out_of_range("Invalid MmapFile::MapView() call. Please try again.");

		auto pMmap = mmap(nullptr, length, PROT_READ, MAP_PRIVATE | MAP_POPULATE , fd, offset);
		if(pMmap == MAP_FAILED) {
            printf("fuuuuck.\n");
			throw std::system_error { errno, std::generic_category() };
		}

		return MappedView(reinterpret_cast<u8*>(pMmap), length);
	}

	MmapFile::MappedView MmapFile::MapWholeView() {
        return MapView(wholeFileSize, 0);
	}

} // namespace core