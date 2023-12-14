/*
* File:		RE/Core/MVectors.h
* Author:	@krovee
* Date:		12/13/23
*
* Desc:		Math linear algebra vectors (2-4d)
*/
#pragma once

#include <cmath>
#include <initializer_list>

namespace re::math {
	template<size_t N>
	struct vec;

	template<>
	struct vec<2> {

		constexpr vec() noexcept
			: iarray()
		{}
		inline ~vec() noexcept = default;

		constexpr vec(float _v_) noexcept
			: x(_v_), y(_v_)
		{}
		constexpr vec(std::int32_t _iv_) noexcept
			: ix(_iv_), iy(_iv_)
		{}
		constexpr vec(float _x_, float _y_) noexcept
			: x(_x_), y(_y_)
		{}
		constexpr vec(std::int32_t _ix_, std::int32_t _iy_) noexcept
			: ix(_ix_), iy(_iy_)
		{}
		constexpr vec(std::initializer_list<float> list) noexcept
			: x(*(list.begin() + 0)), y(*(list.begin() + 1))
		{}
		constexpr vec(std::initializer_list<std::int32_t> list) noexcept
			: ix(*(list.begin() + 0)), iy(*(list.begin() + 1))
		{}

		auto& operator[](size_t i) noexcept {
			return farray[i];
		}
		auto const& operator[](size_t i) const noexcept {
			return farray[i];
		}
		auto& at(size_t i) noexcept {
			return farray[i];
		}
		auto const& at(size_t i) const noexcept {
			return farray[i];
		}
		auto& ati(size_t i) noexcept {
			return iarray[i];
		}
		auto const& ati(size_t i) const noexcept {
			return iarray[i];
		}

		union {
			struct {
				float x;
				float y;
			};
			struct {
				float u;
				float v;
			};
			struct {
				std::int32_t ix;
				std::int32_t iy;
			};
			float farray[2];
			std::int32_t iarray[2];
		};
	};

	template<>
	struct vec<4> {

		constexpr vec() noexcept
			: iarray()
		{}
		inline ~vec() noexcept = default;

		constexpr vec(float _v_) noexcept
			: x(_v_), y(_v_), z(_v_), w(_v_)
		{}
		constexpr vec(std::int32_t _iv_) noexcept
			: ix(_iv_), iy(_iv_), iz(_iv_), iw(_iv_)
		{}
		constexpr vec(float _x_, float _y_, float _z_, float _w_) noexcept
			: x(_x_), y(_y_), z(_z_), w(_w_)
		{}
		constexpr vec(std::int32_t _ix_, std::int32_t _iy_, std::int32_t _iz_, std::int32_t _iw_) noexcept
			: ix(_ix_), iy(_iy_), iz(_iz_), iw(_iw_)
		{}
		constexpr vec(std::initializer_list<float> list) noexcept
			: x(*(list.begin() + 0)), y(*(list.begin() + 1)),
			  z(*(list.begin() + 2)), w(*(list.begin() + 3))
		{}
		constexpr vec(std::initializer_list<std::int32_t> list) noexcept
			: ix(*(list.begin() + 0)), iy(*(list.begin() + 1)),
			  iz(*(list.begin() + 2)), iw(*(list.begin() + 3))
		{}

		auto& operator[](size_t i) noexcept {
			return farray[i];
		}
		auto const& operator[](size_t i) const noexcept {
			return farray[i];
		}
		auto& at(size_t i) noexcept {
			return farray[i];
		}
		auto const& at(size_t i) const noexcept {
			return farray[i];
		}
		auto& ati(size_t i) noexcept {
			return iarray[i];
		}
		auto const& ati(size_t i) const noexcept {
			return iarray[i];
		}

		union {
			struct {
				float x;
				float y;
				float z;
				float w;
			};
			struct {
				float u;
				union {
					struct {
						float v;
						float s;
					};
					vec<2> yz;
				};
				float t;
			};
			struct {
				std::int32_t ix;
				std::int32_t iy;
				std::int32_t iz;
				std::int32_t iw;
			};
			struct {
				vec<2> xy;
				vec<2> zw;
			};
			float			farray[4];
			std::int32_t	iarray[4];
		};
	};

	using vec2 = vec<2>;
	using vec4 = vec<4>;

	static inline vec<2> operator+(vec<2> const& a, vec<2> const& b) noexcept {
		return {
			a.farray[0] + b.farray[0],
			a.farray[1] + b.farray[1]
		};
	}
	static inline vec<2> operator+(vec<2> const& a, float f) noexcept {
		return {
			a.farray[0] + f,
			a.farray[1] + f
		};
	}
	static inline vec<2> operator+(float f, vec<2> const& b) noexcept {
		return {
			b.farray[0] + f,
			b.farray[1] + f
		};
	}
	static inline vec<2> operator-(vec<2> const& a, vec<2> const& b) noexcept {
		return {
			a.farray[0] - b.farray[0],
			a.farray[1] - b.farray[1]
		};
	}
	static inline vec<2> operator-(vec<2> const& a, float f) noexcept {
		return {
			f - a.farray[0],
			f - a.farray[1]
		};
	}
	static inline vec<2> operator-(float f, vec<2> const& b) noexcept {
		return {
			f - b.farray[0],
			f - b.farray[1]
		};
	}
	static inline vec<2> operator*(vec<2> const& a, vec<2> const& b) noexcept {
		return {
			a.farray[0] * b.farray[0],
			a.farray[1] * b.farray[1]
		};
	}
	static inline vec<2> operator*(vec<2> const& a, float f) noexcept {
		return {
			f * a.farray[0],
			f * a.farray[1]
		};
	}
	static inline vec<2> operator*(float f, vec<2> const& b) noexcept {
		return {
			f * b.farray[0],
			f * b.farray[1]
		};
	}
	static inline vec<2> operator/(vec<2> const& a, vec<2> const& b) noexcept {
		return {
			a.farray[0] / b.farray[0],
			a.farray[1] / b.farray[1]
		};
	}
	static inline vec<2> operator/(vec<2> const& a, float f) noexcept {
		return {
			a.farray[0] / f,
			a.farray[1] / f
		};
	}
	static inline vec<2> operator/(float f, vec<2> const& b) noexcept {
		return {
			f / b.farray[0],
			f / b.farray[1]
		};
	}
	static inline vec<2> operator%(vec<2> const& a, vec<2> const& b) noexcept {
		return {
			a.iarray[0] % b.iarray[0],
			a.iarray[1] % b.iarray[1]
		};
	}
	static inline vec<2> operator%(vec<2> const& a, std::int32_t i) noexcept {
		return {
			a.iarray[0] % i,
			a.iarray[1] % i
		};
	}
	static inline vec<2> operator%(std::int32_t i, vec<2> const& b) noexcept {
		return {
			i % b.iarray[0],
			i % b.iarray[1]
		};
	}
	static inline vec<4> operator+(vec<4> const& a, vec<4> const& b) noexcept {
		return {
			a.farray[0] + b.farray[0],
			a.farray[1] + b.farray[1],
			a.farray[2] + b.farray[2],
			a.farray[3] + b.farray[3]
		};
	}
	static inline vec<4> operator+(vec<4> const& a, float f) noexcept {
		return {
			a.farray[0] + f,
			a.farray[1] + f,
			a.farray[2] + f,
			a.farray[3] + f
		};
	}
	static inline vec<4> operator+(float f, vec<4> const& b) noexcept {
		return {
			b.farray[0] + f,
			b.farray[1] + f,
			b.farray[2] + f,
			b.farray[3] + f
		};
	}
	static inline vec<4> operator-(vec<4> const& a, vec<4> const& b) noexcept {
		return {
			a.farray[0] - b.farray[0],
			a.farray[1] - b.farray[1],
			a.farray[2] - b.farray[2],
			a.farray[3] - b.farray[3]
		};
	}
	static inline vec<4> operator-(vec<4> const& a, float f) noexcept {
		return {
			f - a.farray[0],
			f - a.farray[1],
			f - a.farray[2],
			f - a.farray[3]
		};
	}
	static inline vec<4> operator-(float f, vec<4> const& b) noexcept {
		return {
			f - b.farray[0],
			f - b.farray[1],
			f - b.farray[2],
			f - b.farray[3]
		};
	}
	static inline vec<4> operator*(vec<4> const& a, vec<4> const& b) noexcept {
		return {
			a.farray[0] * b.farray[0],
			a.farray[1] * b.farray[1],
			a.farray[2] * b.farray[2],
			a.farray[3] * b.farray[3]
		};
	}
	static inline vec<4> operator*(vec<4> const& a, float f) noexcept {
		return {
			f * a.farray[0],
			f * a.farray[1],
			f * a.farray[2],
			f * a.farray[3]
		};
	}
	static inline vec<4> operator*(float f, vec<4> const& b) noexcept {
		return {
			f * b.farray[0],
			f * b.farray[1],
			f * b.farray[2],
			f * b.farray[3]
		};
	}
	static inline vec<4> operator/(vec<4> const& a, vec<4> const& b) noexcept {
		return {
			a.farray[0] / b.farray[0],
			a.farray[1] / b.farray[1],
			a.farray[2] / b.farray[2],
			a.farray[3] / b.farray[3]
		};
	}
	static inline vec<4> operator/(vec<4> const& a, float f) noexcept {
		return {
			a.farray[0] / f,
			a.farray[1] / f,
			a.farray[2] / f,
			a.farray[3] / f
		};
	}
	static inline vec<4> operator/(float f, vec<4> const& b) noexcept {
		return {
			f / b.farray[0],
			f / b.farray[1],
			f / b.farray[2],
			f / b.farray[3]
		};
	}
	static inline vec<4> operator%(vec<4> const& a, vec<4> const& b) noexcept {
		return {
			a.iarray[0] % b.iarray[0],
			a.iarray[1] % b.iarray[1],
			a.iarray[2] % b.iarray[2],
			a.iarray[3] % b.iarray[3]
		};
	}
	static inline vec<4> operator%(vec<4> const& a, std::int32_t i) noexcept {
		return {
			a.iarray[0] % i,
			a.iarray[1] % i,
			a.iarray[2] % i,
			a.iarray[3] % i
		};
	}
	static inline vec<4> operator%(std::int32_t i, vec<4> const& b) noexcept {
		return {
			i % b.iarray[0],
			i % b.iarray[1],
			i % b.iarray[2],
			i % b.iarray[3]
		};
	}



}
