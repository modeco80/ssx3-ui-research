#pragma once

#include <core/offsetptr.hpp>
#include <core/types.hpp>
#include <span>
#include <string_view>

#include <lui/structs/common.hpp>

namespace lui::structs {

	struct AnimationBlockEntry;
	struct ScreenBlockEntry;
	struct TextureBlockEntry;
	struct FontBlockEntry;

	struct Header {
		u32 magic;
		f32 cookie; // 3.9

		core::OffsetPtr<BlockArray<AnimationBlockEntry>> animationBlock;
		core::OffsetPtr<BlockArray<ScreenBlockEntry>> screenBlock;
		core::OffsetPtr<BlockArray<TextureBlockEntry>> textureBlock;
		core::OffsetPtr<BlockArray<FontBlockEntry>> fontBlock;

		constexpr bool Valid() {
			if(magic != 0x00465949)
				return false;

			// Cranky but this makes it not float math which could fall apart.
			return std::bit_cast<u32>(cookie) == 0x4079999a;
		}

		auto AnimationBlock() {
			return animationBlock(this);
		}

		auto ScreenBlock() {
			return screenBlock(this);
		}

		auto TextureBlock() {
			return textureBlock(this);
		}

		auto FontBlock() {
			return fontBlock(this);
		}
	};

	struct AnimationBlockEntry {
		u32 unk;
		u32 unk2;
		u32 hashName;
		u32 animDataSize;

		std::span<u8> GetAnimationDataPtr() {
			auto* pAnimDataStart = reinterpret_cast<u8*>(this + 1);
			return { pAnimDataStart, animDataSize - sizeof(*this) };
		}

		AnimationBlockEntry* Next() {
			return reinterpret_cast<AnimationBlockEntry*>(reinterpret_cast<u8*>(this + 1) + animDataSize - sizeof(*this));
		}
	};

	struct ScreenBlockEntry {
		u32 hashName;
		u32 screenDataRelativeOffset;

		u8* GetCompressedDataPtr(Header* pHeader) {
			return reinterpret_cast<u8*>(pHeader->ScreenBlock()) + screenDataRelativeOffset;
		}
	};

	struct TextureBlockEntry {
		u32 hashName;

		u8 pad;
		u8 textureId;
		u8 pad2;
		u8 pad3;

		f32 x;
		f32 y;
		f32 w;
		f32 h;
	};

	struct FontBlockEntry {
		u32 fontNameHash;
		u32 fontNameLength;

		const std::string_view GetName() {
			return { reinterpret_cast<const char*>(this + 1), fontNameLength };
		}

		FontBlockEntry* Next() {
			return reinterpret_cast<FontBlockEntry*>(reinterpret_cast<u8*>(this + 1) + fontNameLength);
		}
	};

	
	static_assert(sizeof(Header) == 0x18, "fucking cmon here people");
	static_assert(sizeof(AnimationBlockEntry) == 0x10, "fucking cmon here people");
	static_assert(sizeof(ScreenBlockEntry) == 0x8, "fucking cmon here people");
	static_assert(sizeof(TextureBlockEntry) == 0x18, "fucking cmon here people");
	static_assert(sizeof(FontBlockEntry) == 0x8, "fucking cmon here people");

} // namespace lui::structs