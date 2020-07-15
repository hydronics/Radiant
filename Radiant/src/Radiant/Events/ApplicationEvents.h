#pragma once

#include "Event.h"

namespace Radiant {
	
	class RADIANT_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class RADIANT_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_width(width)
			, m_height(height) 
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent (w,h): " << "(" << m_width << ", " << m_height << ")";
			return ss.str();
		}

		inline unsigned int GetWidth() { return m_width; }
		inline unsigned int GetHeight() { return m_height; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_width, m_height;
	};


	class RADIANT_API WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(unsigned int x, unsigned int y)
			: m_x(x)
			, m_y(y) 
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMoveEvent (x,y): " << "(" << m_x << ", " << m_y << ")";
			return ss.str();

		}
		
		EVENT_CLASS_TYPE(WindowMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_x, m_y;
	};

}
