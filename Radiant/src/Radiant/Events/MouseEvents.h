#pragma once

#include "Event.h"

namespace Radiant {

	// Mouse movement
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_x(x)
			, m_y(y)
		{}

		inline float GetX() { return m_x; }
		inline float GetY() { return m_y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_x << " , " << m_y << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMove)

	private:
		float m_x, m_y;
	};


	// Mouse button base event
	class MouseButtonEvent : public Event
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
	class MouseClickedEvent : public MouseButtonEvent
	{
	public:
		MouseClickedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseClickedEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseButtonClick)
	};

	// Mouse button released
	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << m_buttoncode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseButtonRelease)
	};

	// Mouse scroll wheel
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			: m_xoffset(xoffset)
			, m_yoffset(yoffset)
		{}

		inline float GetXOffset() { return m_xoffset; }
		inline float GetYOffset() { return m_yoffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_xoffset << " , " << m_xoffset << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseScroll)

	private:
		float m_xoffset, m_yoffset;
	};
}
