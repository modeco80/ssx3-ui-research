#pragma once

#include <cassert>
#include <lui/structs/common.hpp>
#include <type_traits>
#include "core/offsetptr.hpp"

namespace lui::structs {

	// TODO: Probably move out of here.
	enum class eUIObjectType : u16 {
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
		eUIObjectType type;
		u16 size;

		u32 hashName;

		// flags/bitmasks:
		// & 0x1f  = draw layer
		// & 0x40  = orientation
		// & 0x80  = wrap
		// & 0x200 = visible
		u32 flags;

		u32 pad[5];

		template <class T>
		auto* CastTo()
			requires(std::is_base_of_v<tUIBaseObject, T>)
		{
			if(type != T::kType) {
				return nullptr;
			}

			return reinterpret_cast<T*>(this);
		}
	};

	struct tTextObject : tUIBaseObject {
		constexpr static auto kType = eUIObjectType::kText;

		u16 fontIndex;

		// flags:
		// & 1 = shadow
		u16 textFlags;

		u32 stringHashName;

		u8 shadowColorA;
		u8 shadowColorR;
		u8 shadowColorG;
		u8 shadowColorB;
	};

	struct ScreenHeader {
		/// Duplicated from the screen table.
		/// Probably to be extra-sure if I had to gues..
		u32 screenHashName;

        core::OffsetPtr<BlockArray<tUIBaseObject>> objectTable;
        // another block array and another optional one.
	};

} // namespace lui::structs