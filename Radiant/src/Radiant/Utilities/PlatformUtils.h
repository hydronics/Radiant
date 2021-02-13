#pragma once

#include <string>

namespace Radiant {

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* fileFilter);
		static std::string SaveFile(const char* fileFilter);
	};

}
