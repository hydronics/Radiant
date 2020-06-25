#pragma once

#include "../Core.h"
#include <string>
#include "../Events/Event.h"

namespace Radiant {

	class RADIANT_API Layer
	{
	public:
#ifdef RD_DEBUG
		Layer(const std::string& name = "a_layer") : m_debug_name(name) {}
#else
		Layer() {}
#endif
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach()	{}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

#ifdef RD_DEBUG
	protected:
		std::string m_debug_name;
#endif // RD_DEBUG
	};

}
