#pragma once

#include <string>
#include <vector>

#include "Layer.h"

#include "Radiant/Events/Event.h"


namespace Radiant {

	class  LayerStack
	{
		using Stack = std::vector<Layer*>;

		using StackIter = Stack::iterator;
		using ConstStackIter = Stack::const_iterator;

		using StackRIter = Stack::reverse_iterator;
		using ConstStackRIter = Stack::const_reverse_iterator;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		StackIter begin() { return m_layers.begin(); }
		StackIter end() { return m_layers.end(); }
		StackRIter rbegin() { return m_layers.rbegin(); }
		StackRIter rend() { return m_layers.rend(); }

		ConstStackIter begin() const { return m_layers.begin(); }
		ConstStackIter end() const { return m_layers.end(); }
		ConstStackRIter rbegin() const { return m_layers.rbegin(); }
		ConstStackRIter rend() const { return m_layers.rend(); }

	private:
		Stack m_layers;
		unsigned int  m_layer_insert_index = 0;
	};

}
