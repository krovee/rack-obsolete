#include <RE/Core/Algorithms.h>

#include <string.h>

namespace re {
	size_t memcount(std::int8_t* bytes_array) noexcept {
		return ::strlen(reinterpret_cast<char*>(bytes_array));
	}
}
