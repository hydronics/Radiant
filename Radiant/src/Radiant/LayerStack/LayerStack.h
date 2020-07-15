#pragma once

#include <string>
#include <vector>

#include "../Core.h"
#include "../Events/Event.h"
#include "Layer.h"

namespace Radiant {

	class RADIANT_API LayerStack
	{
		using Stack = std::vector<Layer*>;
		using StackIter = Stack::iterator;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		StackIter begin() { return m_layers.begin(); }
		StackIter end() { return m_layers.end(); }

	private:
		Stack m_layers;
		StackIter m_layer_insert;
	};

}