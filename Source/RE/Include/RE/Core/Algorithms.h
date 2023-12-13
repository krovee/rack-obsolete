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
}
