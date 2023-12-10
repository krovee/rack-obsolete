/*
* File:		RE/Core/Exports.h
* Author:	@krovee
* Date:		12/10/23
* 
* Desc:		Macro definition to link symbols to executable.
*/
#pragma once

#if		defined(_DLL)
/*
* Macro definition to link symbols to executable.
*/
#	define RE_API	__declspec(dllexport)
#else
/*
* Macro definition to link symbols to executable.
*/
#	define RE_API	__declspec(dllimport)
#endif
