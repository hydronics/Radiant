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

#ifdef RD_ENABLE_ASSERTS
	#define RD_CORE_ASSERT(x, ...) { if(!(x)) { RD_CORE_ERROR("Assertion FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RD_CLIENT_ASSERT(x, ...) { if(!(x)) { RD_CLIENT_ERROR("Assertion FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RD_CORE_ASSERT(x, ...)
	#define RD_CLIENT_ASSERT(x, ...)
#endif

#define RD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)
