#pragma once

#include "Radiant/Input.h"

namespace Radiant {

	class Win32Input : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonClickedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};
}