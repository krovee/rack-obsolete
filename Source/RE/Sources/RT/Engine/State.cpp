#include <RE/RT/Engine/State.h>

namespace re {
	engine_state::engine_state() noexcept {
		// create: 
		//	-> initialize logger system
		//	-> initialize filesystem routines
		//	-> read configs into memory
		//	-> create desktop window
		//	-> initialize graphics
		//	-> initialize physics (global static things)
		//	-> initialize scripts (global static things)
		//	-> initialize world (local physics)
		//	-> initialize world (local scripts)
	}
	engine_state::~engine_state() noexcept {

	}
	bool engine_state::is_full_initialized() const noexcept {
		return
			is_minimal_initialized()
		;
	}
	bool engine_state::is_minimal_initialized() const noexcept {
		return
			_initialized.b_memory
			;
	}
}
