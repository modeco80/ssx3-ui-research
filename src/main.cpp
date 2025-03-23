
#include <core/mmap_file.hpp>
#include <lui/structs/lui_file.hpp>
#include <cstdio>
#include <core/refpack.hpp>
#include <core/stringprintf.hpp>
#include <core/file.hpp>
#include <core/hashname.hpp>

int main(int argc, char** argv) {
	auto mm = core::MmapFile::Open("/home/lily/ssx3ui/FE.LUI");
	auto view = mm.MapWholeView();
	auto viewSpan = view.GetView();

	auto* pLuiHeader = reinterpret_cast<lui::structs::Header*>(viewSpan.data());
    if(!pLuiHeader->Valid()) {
        printf("Invalid LUI file\n");
        return 1;
    }

    // font block
    auto* pFontBlock = pLuiHeader->FontBlock();
    auto* pFontBlockPtr = pFontBlock->GetBlockPtr();
    auto* pFontEnt = pFontBlock->GetBlockPtr();
    for(auto i = 0; i < pFontBlock->elemCount; ++i) {
        auto name = pFontEnt->GetName();
        printf("Font %d: %s\n", i, name.data());
        pFontEnt = pFontEnt->Next();
    }



	auto* pScreenBlock = pLuiHeader->ScreenBlock();
    auto* pScreensPtr = pScreenBlock->GetBlockPtr();

	for(auto i = 0; i < pScreenBlock->elemCount; ++i) {
        auto& screenEntry = pScreensPtr[i];


		std::printf(
		"Screen %d:" "Hash %08x\n",

		i,
		screenEntry.hashName);


        if(screenEntry.hashName == core::GetHashValue32("06title")) {
            printf("found \"06title\"\n");
        }

        auto ptr = screenEntry.GetCompressedDataPtr(pLuiHeader);

        if(*reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)) != 0xfb10) {
            printf("UH OH!!! %04x\n", *reinterpret_cast<u16*>(screenEntry.GetCompressedDataPtr(pLuiHeader)));
        }

#if 0
        auto refDecompressed = core::refpack::Decompress(ptr);
        auto format = core::StringPrintf("screen_%04d_%08x.bin", i, screenEntry.hashName);
        auto file = core::File::Open(format.c_str(), O_RDWR|O_CREAT);
        file.Write(&refDecompressed[0], refDecompressed.size());
        printf("wrote \"%s\"\n", format.c_str());
#endif
	}

	return 0;
}