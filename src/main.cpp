
#include <core/mmap_file.hpp>
#include <lui/structs/lui_file.hpp>
#include <cstdio>
#include <core/refpack.hpp>
#include <core/stringprintf.hpp>
#include "core/file.hpp"

int main(int argc, char** argv) {
	auto mm = core::MmapFile::Open("/home/lily/ssx3ui/FE.LUI");
	auto view = mm.MapWholeView();
	auto viewSpan = view.GetView();

	auto* pLuiHeader = reinterpret_cast<lui::structs::Header*>(viewSpan.data());

	auto* pScreenBlock = pLuiHeader->ScreenBlock();

    auto* pScreensPtr = pScreenBlock->GetBlockPtr();

	for(auto i = 0; i < pScreenBlock->elemCount; ++i) {
        auto& screenEntry = pScreensPtr[i];


		std::printf(
		"Screen %d:" "Hash %08x\n",

		i,
		screenEntry.hashName);


        if(screenEntry.hashName == 0x08065fde) {
            printf("found \"07main_men\"\n");
        }

        auto ptr = screenEntry.GetCompressedDataPtr(pLuiHeader);

        if(*reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)) != 0xfb10) {
            printf("UH OH!!! %04x\n", *reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)));
        }

        auto refDecompressed = core::refpack::Decompress(ptr);

        auto format = core::StringPrintf("screen_%04d_%08x.bin", i, screenEntry.hashName);

        auto file = core::File::Open(format.c_str(), O_RDWR|O_CREAT);
        file.Write(&refDecompressed[0], refDecompressed.size());

        printf("wrote \"%s\"\n", format.c_str());
	}

	return 0;
}