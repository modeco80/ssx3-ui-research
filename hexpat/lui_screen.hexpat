import std.io;

struct UIBlock<T> {
    u32 count;
    T objects[count];
};

// I think?
struct tUIVertex {
    u8 x;
    u8 y;
};

enum eUIObjectType : u16 {
    kGroup = 0x10,
    kIcon = 0x11,
    kListBox = 0x12,
    kMenu = 0x13,
    
    kPair = 0x15,
    kSlider = 0x16,
    kText = 0x17,
    kVector = 0x18,
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
           
    u32 paddingSpace[5]; // maybe for UI Internal usage?
    
    std::print(
        "OBJECT @ 0x{:08x}: name: 0x{:08x}, type: {}, visible: {}, wrap: {}, orientation: {}, drawlayer: {}",
        $,
        hashName,
        objectType,
        (flags & 0x200) ? "ya" : "no",
        (flags & 0x80) ? "ya" : "no",
        (flags & 0x40) != 0 ,
        (flags & 0x1f)
    );
    
    // TODO: Parse every object type
    
    if(objectType == eUIObjectType::kGroup) {
        u32 nrGroupItems;
        u32 groupObjectHashes[nrGroupItems];
    }
    
    else if(objectType == eUIObjectType::kIcon) {
        u32 iconFlags;
        u32 textureElementHashName;
        
        // UV tiling.
        // these are / 100.
        u16 tileU;
        u16 tileV;
    }  
    
    else if(objectType == eUIObjectType::kListBox) {
        u16 listBoxFontIndex; // index in LUI Font block
        u16 listBoxFlags;
        
        if(listBoxFlags & 1)
            std::print("ListBox has shadow!");
        
        u8 listBoxShadowColorA;
        u8 listBoxShadowColorR;
        u8 listBoxShadowColorG;
        u8 listBoxShadowColorB;
    }
    
    
    else if(objectType == eUIObjectType::kMenu) {
        u32 menuNumThings;
        u32 menuThingHashNames[menuNumThings];
    }
    
    else if(objectType == eUIObjectType::kPair) {
        u32 pairEntryHashName;
        u32 pairWidgetHashName;
    }
        
    else if(objectType == eUIObjectType::kSlider) {
        u32 sliderUnk;
        u32 sliderBackgroundHashName;
        u32 sliderKnobHashName;
    }
    
    else if(objectType == eUIObjectType::kText) {
        u16 fontIndex;
        
        // & 1 = shadow
        u16 textFlags;
        u32 stringHashName;
        
        u8 shadowColorA;
        u8 shadowColorR;
        u8 shadowColorG;
        u8 shadowColorB;
        
        if(textFlags & 1) {
            std::print("text has shadow");
        }
    } 
    
    else if(objectType == eUIObjectType::kVector) {
       u8 nrVerts;
       u8 vertexFlagsHigh;
       u8 vertexFlagsLow;
       u8 unk;
       u8 data[objectSize - 0x24];
    } 
    
    
    else if(objectType == eUIObjectType::kVector) {
       u8 nrVerts;
       u8 vertexFlagsHigh;
       u8 vertexFlagsLow;
       u8 unk;
       u8 data[objectSize - 0x24];
    } 
     
     else {
        // TODO: Parse based on size
        u8 data[objectSize - 0x20];
    }
} [[single_color]];

// Frame (keyframes)

// TERMS
// KCP - Keyframe Control Program
//          a very simple VM. 
//        Not turing complete thank fucking god

// KCP  opcode. Split into groups to make it less annoying
enum eKCPOpCode : u8 {
    kKeyframePause = 0x10,
    kKeyframeJump = 0x11, // Haven't seen this yet
    kKeyframeSpawn = 0x12, // Nor this
    
    kObjectAnim = 0x20,
    kObjectSetPos = 0x21, // This got overloaded to basically be a property tag system,
                          // which is probably why kObjectVisible and such got no-oped
    kObjectVisible = 0x22, // Unused, no op in game
    
    kEventTrigger = 0x30, // Triggers a custom event. Only used by the frontend?
    kEventAudio = 0x31,   // Seems to be unused
    
    kScreenStart = 0x40,
    kScreenEnd = 0x41,
    kScreenFocusGain = 0x42,
    kScreenFocusLose = 0x43
};

struct SetPosValue {
    // TODO: enum
    //
    //
    // 0xd - 0x10: RGBA color
    u16 targetId; 
    u16 targetValue;
} [[single_color]];

struct tUIKCPBase {
    eKCPOpCode opcode [[color("00FFFF")]];
    padding [1];
    u16 size;
    
    // Keyframe
    
    if(opcode == eKCPOpCode::kKeyframePause) {
        // No data 
        std::print("{:08x} keyframePause", $);
    }
    
    else if(opcode == eKCPOpCode::kKeyframeJump) {
        std::print("{:08x} keyframeJump", $);
    }
    
    else if(opcode == eKCPOpCode::kKeyframeSpawn) {
        std::print("{:08x} keyframeSpawn", $);
    }
    
    // Object
    
    else if (opcode == eKCPOpCode::kObjectAnim) {
        u32 animationHashName; // in LUI Animation Block.
        u32 objectHashName;
        u32 unk; // play flags?
        
        std::print("{:08x} objectAnimate {:08x} {:08x} {:08x}", $, objectHashName, animationHashName, unk);
    }
    
    else if (opcode == eKCPOpCode::kObjectSetPos) {
        u32 objectTargetHashName;
        u32 valueCount;
        SetPosValue values[valueCount];
        std::print("{:08x} objectSetPos {:08x} {:02d}", $, objectTargetHashName, valueCount);
    }
    
    else if (opcode == eKCPOpCode::kObjectVisible) {
        std::print("{:08x} objectVisible", $);
    }
    
    // Event
    
    else if(opcode == eKCPOpCode::kEventTrigger) {
        u32 eventTriggerHashName;
        std::print("{:08x} eventTrigger {:08x}", $, eventTriggerHashName);
    }
    
    else if(opcode == eKCPOpCode::kEventAudio) {
        u32 hash;
        std::print("{:08x} eventAudio", $);
    }
    
    // Screen
    
    else if(opcode == eKCPOpCode::kScreenStart) {
        // Once contained something. Now it doesn't seem to?
        u32 screenToStart;
        
        std::print("{:08x} screenStart {:08x}", $, screenToStart);
    }
    
    else if(opcode == eKCPOpCode::kScreenEnd) {
        // Once contained something. Now it doesn't seem to?
        u32 screenToEnd;
        
        
        std::print("{:08x} screenEnd {:08x}", $, screenToEnd);
    }
    
    else if(opcode == eKCPOpCode::kScreenFocusGain) {
        // Once contained something. Now it doesn't seem to?
        u32 screenToFocus;
        
        std::print("{:08x} gainFocus {:08x}", $, screenToFocus);
    }
    
    else if(opcode == eKCPOpCode::kScreenFocusLose) {
        // Once contained something. Now it doesn't seem to?
        u32 screenToUnFocus;
        
        
        std::print("{:08x} loseFocus {:08x}",$, screenToUnFocus);
    }
};

struct tUIFrameHeader {
    u32 unk;
    u32 dataSize;
    u16 frameIndex;
    u16 nrKCPInsts;
    
    std::print("KCP PROGRAM for frame {:4d}", frameIndex);
    tUIKCPBase a[nrKCPInsts];
    std::print("END KCP PROGRAM for frame {:4d}", frameIndex);
};


struct tUIScreenBase {
    u32 hashName; // Duplicated. Probably jus tto be sure
    
    u32 objectBlockOffset;
    u32 frameBlockOffset;
    u32 unkOffset3; // not sure what this is because it doesn't seem to appear in retail screens
    
    UIBlock<tUIBaseObject> objectBlock @ objectBlockOffset;
    UIBlock<tUIFrameHeader> frameBlock @ frameBlockOffset;
};

tUIScreenBase screen @ 0;