#pragma once

#include "Core.h"
#include "KeyCodes.h"

namespace Radiant {

	class Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonClicked(int button);
		static std::pair<float,float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}
