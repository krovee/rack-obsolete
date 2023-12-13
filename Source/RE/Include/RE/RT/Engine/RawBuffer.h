/*
* File:		RE/RT/Engine/RawBuffer.h
* Author:	@krovee
* Date:		12/13/23
*
* Desc:
*/
#pragma once

#include <cstdint>

namespace re {
	class raw_buffer {
	public:
		inline raw_buffer() noexcept = default;
		inline ~raw_buffer() noexcept = default;

		inline raw_buffer(char* bytes, size_t const& length) noexcept
			: _bytes(bytes), _size(length)
		{}
		inline raw_buffer(float* fdata, size_t count) noexcept
			:  raw_buffer(reinterpret_cast<char*>(fdata), count * sizeof(float))
		{}
		inline raw_buffer(std::uint8_t* udata, size_t length) noexcept 
			:  raw_buffer(reinterpret_cast<char*>(udata), length)
		{}

		inline raw_buffer(raw_buffer const&) noexcept = default;
		inline raw_buffer(raw_buffer&&) noexcept = default;
		inline raw_buffer& operator=(raw_buffer const&) noexcept = default;
		inline raw_buffer& operator=(raw_buffer&&) noexcept = default;

		inline raw_buffer& assemble(char* bytes, size_t const& length) noexcept {
			_bytes = bytes;
			_size = length;
			return *this;
		}
		inline raw_buffer& assemble(float* fdata, size_t const& count) noexcept {
			_bytes = reinterpret_cast<char*>(fdata);
			_size = count * sizeof(float);
			return *this;
		}
		inline raw_buffer& assemble(std::uint8_t* udata, size_t length) noexcept {
			_bytes = reinterpret_cast<char*>(udata);
			_size = length;
			return *this;
		}

		auto data() noexcept {
			return _bytes;
		}
		auto data() const noexcept {
			return _bytes;
		}
		auto size() const noexcept {
			return _size;
		}

		auto& operator[](size_t i) noexcept {
			return _bytes[i];
		}
		auto const& operator[](size_t i) const noexcept {
			return _bytes[i];
		}
		template<class T = char>
		T& at(size_t i) noexcept {
			return reinterpret_cast<T*>(_bytes)[i];
		}
		template<class T = char>
		T const& at(size_t i) const noexcept {
			return reinterpret_cast<const T*>(_bytes)[i];
		}

	private:
		char* _bytes = nullptr;
		size_t _size  = 0;
	};

}
