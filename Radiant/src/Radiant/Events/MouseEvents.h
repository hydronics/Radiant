#pragma once

#include "Event.h"

namespace Radiant {

	// Mouse movement
	class RADIANT_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(int x, int y)
			: m_x(x)
			, m_y(y)
		{}

		inline float GetX() { return m_x; }
		inline float GetY() { return m_y; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: (" << m_x << " , " << m_y << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMove)

	private:
		float m_x, m_y;
	};


	// Mouse button base event
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
	class RADIANT_API MousePressEvent : public MouseButtonEvent
	{
	public:
		MousePressEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MousePressEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	// Mouse button released
	class RADIANT_API MouseReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseReleaseEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseReleaseEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseButtonRelease)
	};

	// Mouse scroll wheel
	class RADIANT_API MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xoffset, float yoffset)
			: m_xoffset(xoffset)
			, m_yoffset(yoffset)
		{}

		inline float GetX() { return m_xoffset; }
		inline float GetY() { return m_yoffset; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: (" << m_xoffset << " , " << m_xoffset << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScroll)

	private:
		float m_xoffset, m_yoffset;
	};
}