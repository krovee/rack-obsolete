/*
* File:		RE/Core/Algorithms.h
* Author:	@krovee
* Date:		12/12/23
*
* Desc:		
*/
#pragma once

#include <cstdint>

namespace re {
	/*
	* re::memcount(bytes_array) 
	* Algorithmic function for counting number of bytes.
	*/
	size_t memcount(std::int8_t* bytes_array) noexcept;
	
	/*
	* enum re::hash_algorithm {};
	* Enumeration used to determine which algorithm should
	* hash_function class use.
	*/
	enum hash_algorithm : uint8_t {
		/*
		* Famous Murmur algorithm, revision 2. Capable of 
		* both x32 and x64 computations.
		*/
		hash_algorithm_murmur2
	};
	
	/*
	* class re::hash_function {};
	* Utility class for complex-fast hashing function.
	*/
	class hash_function {
	public:
		static size_t	compute  (const void* data, size_t len, size_t	 seed, hash_algorithm algoritm = hash_algorithm_murmur2) noexcept;
		static uint32_t compute32(const void* data, size_t len, uint32_t seed, hash_algorithm algoritm = hash_algorithm_murmur2) noexcept;
	};
}
