#pragma once

#include "Event.h"

namespace Radiant {
	
	class RADIANT_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(unsigned int width, unsigned int height) {}

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class RADIANT_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_width(width)
			, m_height(height) 
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent (w,h): " << "(" << m_width << ", " << m_height << ")";
			return ss.str();

		}

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_width, m_height;
	};


	class RADIANT_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(unsigned int x, unsigned int y)
			: m_x(x)
			, m_y(y) 
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent (x,y): " << "(" << m_x << ", " << m_y << ")";
			return ss.str();

		}
		
		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_x, m_y;
	};

}