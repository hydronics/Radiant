#pragma once

#include "Event.h"

namespace Radiant {

	// Mouse movement
	class RADIANT_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, int y)
			: m_x(x)
			, m_y(y)
		{}

		inline int GetX() { return m_x; }
		inline int GetY() { return m_y; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_x << " , " << m_y << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		int m_x, m_y;
	};


	// Mouse button preseed
	class RADIANT_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() { return m_buttoncode; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int buttoncode)
			: m_buttoncode(buttoncode) {}

		int m_buttoncode;
	};


	// Mouse button pressed
	class RADIANT_API MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MousePressedEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	// Mouse button released
	class RADIANT_API MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	// Mouse scroll wheel
	class RADIANT_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int xoffset, int yoffset)
			: m_xoffset(xoffset)
			, m_yoffset(yoffset)
		{}

		inline int GetX() { return m_xoffset; }
		inline int GetY() { return m_yoffset; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_xoffset << " , " << m_xoffset << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScrolled)

	private:
		float m_xoffset, m_yoffset;
	};
}