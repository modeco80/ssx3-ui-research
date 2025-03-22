#include <core/types.hpp>

namespace lui::structs {

    /// A simple block array.
	template <class T>
	struct BlockArray {
		u32 elemCount;

		T* GetBlockPtr() {
			return reinterpret_cast<T*>(this + 1);
		}
	};

} // namespace lui::structs