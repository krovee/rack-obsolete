/*
* File:		RE/RT/Graphics/DesktopWindow.h
* Author:	@krovee
* Date:		12/13/23
*
* Desc:		
*/
#pragma once

#include <RE/Core/MVectors.h>
#include <RE/Core/Memory.h>
#include <RE/ADT/Flags.h>

namespace re::gfx {
	class desktop_window {
	public:
		struct native_window_handle;

		enum create_flags : uint32_t {
			create_flags_default_style,
			create_flags_borderless_fullscreen_style,

			__create_flags_count
		};
		using create_flags_view = 
			adt::flags_view<create_flags, __create_flags_count>;

		struct create_info {
			const char* p_title;
			math::vec2  i_size;
			create_flags_view flags;

			constexpr create_info() noexcept
				: p_title("rack engine runtime")
				, i_size(1920, 1080)
				, flags(create_flags_borderless_fullscreen_style)
			{}
		};
		
		constexpr desktop_window() noexcept = default;
		~desktop_window() noexcept;

		bool create(create_info const& ci) noexcept;

		bool is_alive() const noexcept;
		void update_events() const noexcept;
		void update_state() noexcept;

		void* get_handle() const noexcept;
		auto get_size() const noexcept {
			return _state.i_size;
		}
		auto is_fullscreen_mode() const noexcept {
			return _state.flags == create_flags_borderless_fullscreen_style;
		}

		struct state : create_info {
			bool b_alive;
			bool b_dirty;

			constexpr state()
				: create_info()
				, b_alive(false)
				, b_dirty(true)
			{}
		};
	private:
		enum {
			NATIVE_WINDOW_HANDLE_SIZE = 128
		};
		low_level::stack_memory_provider<NATIVE_WINDOW_HANDLE_SIZE>
								_native_allocator = {};
		native_window_handle*	_native = nullptr;
		state					_state = {};
	};
}
