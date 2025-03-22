import std.io;

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


struct LuiBlockData<T> { //<T> {
    u32 count;
    T objects[count];
    //T object;
};

fn testString(str test) {
    std::print("hash of \"{}\" is 0x{:08x}", test, GetHashValue32(test));
    return GetHashValue32(test);
};

testString("tReal");
testString("Menu0000");
testString("e7");

enum eUIObjectType : u16 {
    kGroup = 0x10,
    kIcon = 0x11,
    kListBox = 0x12,
    kMenu = 0x13,
    
    kPair = 0x15,
    kSlider = 0x16,
    kText = 0x17,
    kVector = 0x18, // vector bitmap ?
    kProgress = 0x19,
    kTextScroll = 0x20,
    kScroll = 0x21
};

struct tUIBaseObject {
    eUIObjectType objectType;
    u16 objectSize;
    u32 hashName;
    
    // flags/bitmasks:
    // & 0x1f  = draw layer
    // & 0x40  = orientation
    // & 0x80  = wrap
    // & 0x200 = visible
    u32 flags;
    
       
    
    std::print(
        "visible: {}, wrap: {}, orientation: {}, drawlayer: {}"
        ,
        (flags & 0x200) ? "ya" : "no",
        (flags & 0x80) ? "ya" : "no",
        (flags & 0x40) != 0 ,
        (flags & 0x1f)
    );
    
    
    if(objectType == eUIObjectType::kIcon) {
        u32 ptr1; // might be for native pointers.
        u32 ptr2; 
    
        u32 pad3;
        u32 pad4;
        u32 pad5;
        
        u32 unk1;
        u32 textureHashName;
        u32 unk2;
    }  else if(objectType == eUIObjectType::kText) {
        u32 pad[5];
        u16 fontIndex;
        
        // & 1 = shadow
        u16 textFlags;
        u32 stringHashName;
        
        u8 shadowColorA;
        u8 shadowColorR;
        u8 shadowColorG;
        u8 shadowColorB;
    } else {
        // TODO: Parse based on size
        u8 data[objectSize - 0xc];
    }
} ;


struct screenHeader {
    u32 hashName; // Duplicated. Probably jus tto be sure
    
    u32 objectsBlockOffset;
    u32 unkOffset2;
    u32 unkOffset3;
    
    
    LuiBlockData<tUIBaseObject> objectsBlock @ objectsBlockOffset;
    //LuiBlockData unk2 @ unkOffset2;
    
   // if(unkOffset3)
  //      LuiBlockData unk3 @ unkOffset3;
};


screenHeader header @ 0;

std::print("{:08x} {:08x}", 60 << 2, 60 * 4);