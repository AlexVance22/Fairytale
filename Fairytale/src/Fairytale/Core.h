#pragma once

#ifdef FTALE_PLATFORM_WINDOWS
	#ifdef FTALE_BUILD_DLL
		#define FTALE_API __declspec(dllexport)
	#else
		#define FTALE_API __declspec(dllimport)
	#endif
#else
	#error Fairytale only supports windows
#endif