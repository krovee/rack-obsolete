#if defined(_WIN32) || defined(_WIN64)

#include <RE/RT/Graphics/DesktopWindow.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace re::win32 {
	static LRESULT message_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
		auto _state_pointer = reinterpret_cast<gfx::desktop_window::state*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (nullptr != _state_pointer) {
			auto& _state = *_state_pointer;
			switch (Msg) {
				default: break;
				case WM_CLOSE:
				case WM_QUIT: {
					_state.b_alive = false;
					_state.b_dirty = true;
					PostQuitMessage(0);
				} break;
			}
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	static DWORD make_window_create_style(gfx::desktop_window::create_flags_view flags) noexcept {
		if (gfx::desktop_window::create_flags_default_style == flags)
			return WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
		if (gfx::desktop_window::create_flags_borderless_fullscreen_style == flags)
			return WS_POPUP;
		return WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
	}
	static DWORD make_window_show_style(gfx::desktop_window::create_flags_view flags) noexcept {
		if (gfx::desktop_window::create_flags_default_style == flags)
			return SW_SHOW;
		if (gfx::desktop_window::create_flags_borderless_fullscreen_style == flags)
			return SW_SHOWMAXIMIZED;
		return SW_SHOW;
	}
	static math::vec4 make_window_create_coords(gfx::desktop_window::create_info const& ci) {
		RECT rc = { 0, 0, ci.i_size.ix, ci.i_size.iy };
		AdjustWindowRect(&rc, make_window_create_style(ci.flags), FALSE);
		int32_t x = CW_USEDEFAULT, y = CW_USEDEFAULT;
		if (gfx::desktop_window::create_flags_borderless_fullscreen_style == ci.flags) {
			x = y = 0;
		}
		return { x, y, static_cast<int32_t>(rc.right - rc.left), static_cast<int32_t>(rc.bottom - rc.top) };
	}
}

namespace re::gfx {
	struct desktop_window::native_window_handle {
		HWND hwnd;
		WNDCLASSA wca;
		MSG msg;
	};

	desktop_window::~desktop_window() noexcept {
		if (nullptr == _native) {
			return;
		}

		if (_native->hwnd) {
			DestroyWindow(_native->hwnd);
		}

		low_level::oad(_native_allocator, _native);
	}

	bool desktop_window::create(create_info const& ci) noexcept {

		// check for double create call
		if (_native && _native->hwnd) {
			// and if so, destroy an old window
			if (_native->hwnd) {
				DestroyWindow(_native->hwnd);
			}
			// and deallocate native handle
			low_level::oad(_native_allocator, _native);
			// and invalidate native allocator 
			// (cheap because it is stack based)
			_native_allocator = {};
		}

		low_level::oac(_native_allocator, _native);
		if (nullptr == _native) {
			return false;
		}

		auto& wca = _native->wca;
		wca.hInstance = GetModuleHandle(nullptr);
		wca.lpfnWndProc = win32::message_proc;
		wca.lpszClassName = "re_desktop_window";
		wca.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		const auto atm = RegisterClass(&wca);
		if (INVALID_ATOM == atm) {
			return false;
		}

		auto& hwnd = _native->hwnd;
		const auto dwStyle = win32::make_window_create_style(ci.flags);
		const auto coords = win32::make_window_create_coords(ci);
		_state.i_size = coords.zw;

		hwnd = CreateWindow("re_desktop_window", ci.p_title, dwStyle, 
			coords.ix, coords.iy,
			coords.iz, coords.iw,
			nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&_state));
		ShowWindowAsync(hwnd, win32::make_window_show_style(ci.flags));

		UnregisterClass("re_desktop_window", GetModuleHandle(nullptr));

		_state.b_alive = true;
		return true;
	}

	bool desktop_window::is_alive() const noexcept {
		return _state.b_alive;
	}
	void desktop_window::update_events() const noexcept {
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	void desktop_window::update_state() noexcept {
		if (_state.b_dirty) {
			// TODO(krovee): use desktop_window::update_state() somehow..
			_state.b_dirty = false;
		}
	}
	void* desktop_window::get_handle() const noexcept {
		return reinterpret_cast<void*>(const_cast<HWND__*>(_native->hwnd));
	}
}

#endif 
