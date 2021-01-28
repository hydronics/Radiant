#pragma once

#include <memory>

#ifdef RD_ENABLE_ASSERTS
	#define RD_CORE_ASSERT(x, ...) { if(!(x)) { RD_CORE_ERROR("Assertion FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RD_CLIENT_ASSERT(x, ...) { if(!(x)) { RD_CLIENT_ERROR("Assertion FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RD_CORE_ASSERT(x, ...)
	#define RD_CLIENT_ASSERT(x, ...)
#endif

#define RD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1 << x)

namespace Radiant {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#define MAX_SHADERS 2
