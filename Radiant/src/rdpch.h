#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

#include "Radiant/Core/Core.h"
#include "Radiant/Core/Log.h"

#include "Radiant/Profiling/Instrumenter.h"

#ifdef RD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
