#include <RE/RT/Starter.h>
#include <RE/Core/Memory.h>
#include <RE/RT/Engine/State.h>

namespace re {
	namespace {
		low_level::stack_memory_provider<sizeof(engine_state)> s_engine_state_provider = {};
		engine_state* s_engine_state = nullptr;
	}

	main_starter::main_starter(create_info const& ci) noexcept {
		// start code
		low_level::oac(s_engine_state_provider, s_engine_state);
		if (nullptr == s_engine_state) {
			// TODO(krovee): make complete logging system.
		}
	}

	main_starter::~main_starter() noexcept {
		// destroy code
		low_level::oad(s_engine_state_provider, s_engine_state);
	}
}
