
#include <cstdio>
#include <core/hashname.hpp>

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [string to hash]\n", argv[0]);
        return 0;
    }

    auto hashName = core::GetHashValue32(argv[1]);
    printf("%08x\n", hashName);
    return 0;
}