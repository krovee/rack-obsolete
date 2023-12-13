/*
* File:		RE/RT/Engine/State.h
* Author:	@krovee
* Date:		12/13/23
*
* Desc:		
*/
#pragma once

#include <RE/Core/Exports.h>
#include <RE/Core/Memory.h>

namespace re {
	/*
	* class re::engine_state {};
	* Main engine's state charger that controls engine's
	* lifetime and main routines.
	*/
	class engine_state {
	public:

		engine_state() noexcept;
		~engine_state() noexcept;

		/*
		* struct re::engine_state::boot_sequence {};
		* Captures initialization sequence of flags used to 
		* indicate success/fail to engine's boot process.
		*/
		struct boot_sequence {
			/*
			* Indicates if every required allocator is properly
			* initialized and ready to work with.
			*/
			bool b_memory;

			constexpr boot_sequence() noexcept
				: b_memory(false)
			{}
		};

		/*
		* Used to check if all requested at boot stage modules
		* we're initialized properly.
		*/
		bool is_full_initialized() const noexcept;
		/*
		* Tells if engine's minimal set of hardly required
		* modules we're initialized properly.
		*/
		bool is_minimal_initialized() const noexcept;

		auto& mem() noexcept {
			return memman;
		}
		auto const& mem() const noexcept {
			return memman;
		}
	private:
		boot_sequence _initialized = {};
		
		low_level::memory_manager memman = {};
	};
}
