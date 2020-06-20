#pragma once

#ifdef RD_PLATFORM_WINDOWS
	#ifdef RD_BUILD_DLL
		#define RADIANT_API __declspec(dllexport)
	#else
		#define RADIANT_API __declspec(dllimport)
	#endif // RD_BUILD_DLL

#else
	#error Radiant only supports Windows currently.
#endif

#define BIT(x) (1 << x)
