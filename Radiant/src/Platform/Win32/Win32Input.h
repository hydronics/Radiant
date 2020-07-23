#pragma once

#include "Radiant/Core/Input.h"

namespace Radiant {

	class Win32Input : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonClickedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}
