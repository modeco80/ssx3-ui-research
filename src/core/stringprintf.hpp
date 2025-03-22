
#include <cstdarg>
#include <string>
#include <string_view>

namespace core {

    /// Ala to Google StringPrintf().
    std::string StringPrintf(std::string_view format, ...);

}