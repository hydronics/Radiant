#pragma once

#include "Event.h"

namespace Radiant {

	// Key code base - Abstract (doesn't implmement EVENT_CLASS_TYPE methods)
	class KeyboardEvent : public Event
	{
	public:
		inline int GetKeycode() { return m_keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyboardEvent(int keycode)
			: m_keycode(keycode) {}

		int m_keycode;
	};
	
	// Key pressed/repeat
	class KeyboardPressEvent : public KeyboardEvent
	{
	public:
		KeyboardPressEvent(int keycode, int repeat_count)
			: KeyboardEvent(keycode)
			, m_repeated(repeat_count) {}

		inline unsigned int GetRepeatCount() { return m_repeated; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyboardPress event: " << m_keycode << "(repeats: " << m_repeated << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyboardPress)

	private:
		unsigned int m_repeated;
	};

	// Key released
	class KeyboardReleaseEvent : public KeyboardEvent
	{
	public:
		KeyboardReleaseEvent(int keycode, int repeat_count)
			: KeyboardEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyboardRelease event: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyboardRelease)
	};


	// Key pressed/repeat
	class KeyboardTypedEvent : public KeyboardEvent
	{
	public:
		KeyboardTypedEvent(int keycode)
			: KeyboardEvent(keycode) {}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyboardTyped event: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyboardTyped)
	};
}
