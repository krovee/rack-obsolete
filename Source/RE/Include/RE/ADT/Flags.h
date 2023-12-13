/*
* File:		RE/Core/Exports.h
* Author:	@krovee
* Date:		12/13/23
*
* Desc:		
*/
#pragma once

#include <RE/Core/Memory.h>

namespace re::adt {
	/*
	* class re::adt::flags_view<FlagType, NumFlags> {};
	* Simple utility class for debugging purposes to 
	* produce a readable flags with Visual Studio 20XX.
	*/
	template<class FlagType = std::uint32_t, size_t NumFlags = sizeof(FlagType) * 2>
	class flags_view {
	public:
		constexpr flags_view() noexcept = default;
				 ~flags_view() noexcept = default;

		/*
		* Toggles ENABLED a certain  *flag*  .
		*/
		inline void set(FlagType f) noexcept {
			_bits[f] = true;
		}
		/*
		* Toggles DISABLED a certain  *flag*  .
		*/
		inline void reset(FlagType f) noexcept {
			_bits[f] = false;
		}
		/*
		* Toggles DISABLED all flags.
		*/
		inline void reset() noexcept {
			low_level::memset(_bits, 0, NumFlags);
		}

	private:
		bool _bits[NumFlags] = {};
	};
}
