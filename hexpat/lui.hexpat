#include <std/mem.pat>
#include <std/string.pat>

// Steal this implementation if you want
// (thank u otpsp daddy)
fn GetHashValue32(str input) {
    u32 hash = 0;
    u32 high = 0;
    
    u64 len = std::string::length(input);
    
    for(u64 i = 0, i < len, i += 1) {
        hash = (hash << 4) + std::string::at(input, i);
        
        high = hash & 0xf0000000;
        
        if(high)
            hash ^= high >> 23;

        hash &= ~high;
    }

    return hash;
};


fn testString(str test) {
    std::print("hash of \"{}\" is 0x{:08x}", test, GetHashValue32(test));
    return GetHashValue32(test);
};

testString("06title");

struct LuiBlockData<T> {
    u32 count;
    T objects[count];
};

struct LuiAnimation {
    u32 unk;
    u32 unk2;
    u32 hashName;
    u32 dataSize;
    
    u8 animData[dataSize - 0x10]; // PARSE LATER
};

struct LuiScreen {
    u32 hashName;     // hashed (with GetHashValue32) screen name
    u32 screenDataOff;// relative offset to (compressed) object data
                      // relative to the start of this table
    
    if(hashName == GetHashValue32("06title")) {
        std::print("Found main menu screen in table, off is 0x{:08x} (we are at 0x{:08x})", parent.parent.screenBlockOffset + screenDataOff, $);
    }
    
    // screendata is REFPACK compressed,
    // this points to the 10 FB magic.
    u16 thing @ (parent.parent.screenBlockOffset) + screenDataOff;
};
    


struct LuiTexture {
    u32 hashName;
    
    u8 pad;
    u8 textureId;
    u8 pad2;
    u8 pad3;
    
    // rectangle
    float x;
    float y;
    float w;
    float h;
    
    // fe_1.LUI test
    if(hashName == GetHashValue32("macface"))
        std::print("apple {}", textureId);
        
        
         if(hashName == 0x74ae)
        std::print("apple {}", textureId);
};


struct LuiFontName {
    u32 fontNameHash; 
    u32 fontNameLength;
    char name[fontNameLength];
};


struct LuiHeaderBlock {
    u8 magic[4]; // 'IYF '
    float Cookie;  // 3.9 for retail SSX 3
    
    u32 animBlockOffset;
    
    u32 screenBlockOffset;
    
    u32 textureBlockOffset;
    
    u32 fontNameTableOffset;
    
    // data tables.
    LuiBlockData<LuiAnimation> animationBlock @ animBlockOffset;
    LuiBlockData<LuiScreen> screenBlock @ screenBlockOffset;
    LuiBlockData<LuiTexture> textureBlock @ textureBlockOffset;
    LuiBlockData<LuiFontName> fontNameTable @ fontNameTableOffset;
};


LuiHeaderBlock lui @ 0x0;