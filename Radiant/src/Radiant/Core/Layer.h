#pragma once

#include <string>

#include "Radiant/Core/Core.h"
#include "Radiant/Core/Timestep.h"
#include "Radiant/Events/Event.h"

namespace Radiant {

	class Layer
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
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

#ifdef RD_DEBUG
	protected:
		std::string m_debug_name;
#endif // RD_DEBUG
	};

}
