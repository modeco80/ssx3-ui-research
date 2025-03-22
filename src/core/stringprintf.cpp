#include <core/stringprintf.hpp>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace core {

	std::string StringPrintf(std::string_view format, ...) {
        std::string str;
		va_list va;

		va_start(va, format);
		    auto len = vsnprintf(nullptr, 0, format.data(), va);
		    if(len == -1)
    			return "";
		    str.resize(len + 1);
        va_end(va);

		va_start(va, format);
		if(vsnprintf(&str[0], len+1, format.data(), va) == -1)
			return "";
		va_end(va);

		return str;
	}
} // namespace core