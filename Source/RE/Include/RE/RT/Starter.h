/*
* File:		RE/RT/Starter.h
* Author:	@krovee
* Date:		12/10/23
*
* Desc:		
*/
#pragma once

#include <RE/Core/Exports.h>

namespace re {
	/*
	* class re::main_starter {};
	* 
	* Lowest level bootstrapper for entire engine,
	* where all start code executes with constructor,
	* and all destruction happens on destructor.
	*/
	class RE_API main_starter {
	public:
		struct create_info {
			int wndShowCmd = {};
		};

		main_starter(create_info const& ci) noexcept;
		virtual ~main_starter() noexcept;
	};
}
