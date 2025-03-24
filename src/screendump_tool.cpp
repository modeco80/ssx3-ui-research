// Tool which dumps uncompressed screen data.

#include <core/mmap_file.hpp>
#include <lui/structs/lui_file.hpp>
#include <cstdio>
#include <core/refpack.hpp>
#include <core/stringprintf.hpp>
#include <core/file.hpp>
#include <core/hashname.hpp>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::fprintf(stderr, "Invalid usage.\n");
        std::fprintf(stderr, "Usage: %s [path to LUI file to dump screens out of]\n", argv[0]);
        return 1;
    }

	auto mm = core::MmapFile::Open(argv[1]);
	auto view = mm.MapWholeView();
	auto viewSpan = view.GetView();

	auto* pLuiHeader = reinterpret_cast<lui::structs::Header*>(viewSpan.data());
    if(!pLuiHeader->Valid()) {
        printf("Invalid LUI file\n");
        return 1;
    }

	auto* pScreenBlock = pLuiHeader->ScreenBlock();
    auto* pScreensPtr = pScreenBlock->GetBlockPtr();

	for(auto i = 0; i < pScreenBlock->elemCount; ++i) {
        auto& screenEntry = pScreensPtr[i];
		std::printf("Screen %d: Hash %08x\n", i, screenEntry.hashName);

        auto ptr = screenEntry.GetCompressedDataPtr(pLuiHeader);

        if(*reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)) != 0xfb10) {
            printf("Screen %d's compressed data ptr is wrong, led us to start bytes of %04x\n", i, *reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)));
            continue;
        }

        auto refDecompressed = core::refpack::Decompress(ptr);
        auto format = core::StringPrintf("screen_%04d_%08x.bin", i, screenEntry.hashName);
        auto file = core::File::Open(format.c_str(), O_RDWR|O_CREAT);
        file.Write(&refDecompressed[0], refDecompressed.size());
        printf("wrote \"%s\"\n", format.c_str());
	}

	return 0;
}