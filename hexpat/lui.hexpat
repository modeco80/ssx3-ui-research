struct UIBlock<T> {
    u32 count;
    T objects[count];
};

// Animations

struct tUIAnimChannel {
    u16 unk;
    u16 frameOffsetMaybe;
    u16 size;
    u16 numChannelInsts;
    
    // TODO: Figure out what these do
    u32 channelInsts[numChannelInsts];
};

struct tUIAnimation {
    u32 unk;
    u32 nrChannels;
    u32 hashName;
    u32 dataSize;
    
    tUIAnimChannel channels[nrChannels];
};

// Screens

struct tUIScreen {
    u32 hashName;     // hashed (with GetHashValue32) screen name
    u32 screenDataOff;// relative offset to (compressed) object data
                      // relative to the start of this table
                      
    // screendata is REFPACK compressed,
    // this points to the 10 FB magic.
    u16 thing @ (parent.parent.screenBlockOffset) + screenDataOff;
};
    
// Texture

struct tUITexture {
    u32 hashName;
    
    u8 pad;
    u8 textureId;
    u8 pad2;
    u8 pad3;
    
    // rectangle (pretty sure) of 
    // the texture in the given texture
    float x;
    float y;
    float w;
    float h;
};

// Font

struct tUIFont {
    u32 fontNameHash; 
    u32 fontNameLength;
    char name[fontNameLength];
};


struct tUIHeader {
    u8 magic[4]; // 'IYF '
    float Cookie;  // 3.9 for retail SSX 3
    
    u32 animBlockOffset;
    
    u32 screenBlockOffset;
    
    u32 textureBlockOffset;
    
    u32 fontBlockOffset;
    
    //tUIAnimation testAnim @ 0x1c;
    
    // data tables.
    UIBlock<tUIAnimation> animationBlock @ animBlockOffset;
    UIBlock<tUIScreen> screenBlock @ screenBlockOffset;
    UIBlock<tUITexture> textureBlock @ textureBlockOffset;
    UIBlock<tUIFont> fontBlock @ fontBlockOffset;
};


tUIHeader lui @ 0x0;