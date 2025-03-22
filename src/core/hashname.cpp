#include <core/hashname.hpp>

namespace core {

    struct tHashName32 {
        u32 hash;

        void getHashValue(const char* pszIn) {
            u32 h = 0;
            u32 high = 0;
            while(*pszIn) {
                h = (h << 4) + *pszIn++;
                high = h & 0xf0'00'00'00;

                if(high) {
                    h ^= high >> 23;
                }

                h &= ~high;
            }

            hash = h;
        }
    };

    u32 GetHashValue32(std::string_view data) {
        tHashName32 hashName;
        hashName.getHashValue(data.data());
        return hashName.hash;
    }

}