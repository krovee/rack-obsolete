#include <RE/Core/Algorithms.h>

#include <string.h>

namespace re {
	size_t memcount(std::int8_t* bytes_array) noexcept {
		return ::strlen(reinterpret_cast<char*>(bytes_array));
	}

	static constexpr auto rotate_left_32(uint32_t x, int8_t r) noexcept {
		return (x << r) | (x >> (32 - r));
	}
	static constexpr auto rotate_left_64(uint32_t x, int8_t r) noexcept {
		return (x << r) | (x >> (64 - r));
	}
	static constexpr auto rotate_right_32(uint32_t x, int8_t r) noexcept {
		return (x >> r) | (x << (32 - r));
	}
	static constexpr auto rotate_right_64(uint32_t x, int8_t r) noexcept {
		return (x >> r) | (x << (64 - r));
	}

	static size_t	compute_murmur2_64(const void* data, size_t len, size_t   seed) noexcept;
	static uint32_t compute_murmur2_32(const void* data, size_t len, uint32_t seed) noexcept;
	
	using pfn_hash_function_64 = size_t	 (*) (const void* data, size_t len, size_t   seed) noexcept;
	using pfn_hash_function_32 = uint32_t(*) (const void* data, size_t len, uint32_t seed) noexcept;

	pfn_hash_function_64 hash_functions_64[] = {
		compute_murmur2_64,
	};

	pfn_hash_function_32 hash_functions_32[] = {
		compute_murmur2_32,
	};

	size_t hash_function::compute(const void* data, size_t len, size_t seed, hash_algorithm algoritm) noexcept {
		return hash_functions_64[algoritm](data, len, seed);
	}
	uint32_t hash_function::compute32(const void* data, size_t len, uint32_t seed, hash_algorithm algoritm) noexcept {
		return hash_functions_32[algoritm](data, len, seed);
	}

	size_t compute_murmur2_64(const void* data, size_t len, size_t seed) noexcept {
		enum : size_t {
			m = 0xc6a4a7935bd1e995ULL,
			r = 47
		};
		size_t h = seed ^ (len * m);
		auto p = reinterpret_cast<const size_t*>(data);
		auto end = p + (len / 8);

		while (p != end) {
			auto k = *p++;
			
			k *= m;
			k ^= k >> r;
			k *= m;
			h ^= k;
			h *= m;
		}
		
		auto p2 = reinterpret_cast<const uint8_t*>(p);

		switch (len & 7) {
			case 7: h ^= ((size_t)p2[6]) << 48;
			case 6: h ^= ((size_t)p2[5]) << 40;
			case 5: h ^= ((size_t)p2[4]) << 32;
			case 4: h ^= ((size_t)p2[3]) << 24;
			case 3: h ^= ((size_t)p2[2]) << 16;
			case 2: h ^= ((size_t)p2[1]) << 8 ;
			case 1: h ^= ((size_t)p2[0])	  ;
					h *= m;
		};

		h ^= h >> r;
		h *= m;
		h ^= h >> r;

		return h;
	}
	uint32_t compute_murmur2_32(const void* data, size_t len, uint32_t seed) noexcept {
		enum {
			m = 0x5bd1e995u,
			r = 24i32
		};
		uint32_t h = seed ^ static_cast<uint32_t>(len);
		auto p = reinterpret_cast<const uint8_t*>(data);
		while (len >= 4) {
			uint32_t k = *reinterpret_cast<uint32_t*>(const_cast<uint8_t*>(p));
			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			p += 4;
			len -= 4;
		}
		switch (len) {
			case 3: h ^= p[2] << 16;
			case 2: h ^= p[1] << 8;
			case 1: h ^= p[0];
					h *= m;
		}
		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;
		return h;
	}
}
