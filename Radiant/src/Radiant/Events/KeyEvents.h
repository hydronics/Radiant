#pragma once

#include "Event.h"

namespace Radiant {

	// Key code base - Abstract (doesn't implmement EVENT_CLASS_TYPE methods)
	class RADIANT_API KeyEvent : public Event
	{
	public:
		inline int GetKeycode() { return m_keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode) 
			: m_keycode(keycode) {}

		int m_keycode;
	};
	
	// Key pressed/repeat
	class RADIANT_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeat_count)
			: KeyEvent(keycode)
			, m_repeated(repeat_count) {}

		inline unsigned int GetRepeatCount() { return m_repeated; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressed event: " << m_keycode << "(repeats: " << m_repeated << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		unsigned int m_repeated;
	};

	// Key released
	class RADIANT_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeat_count)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleased event: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
