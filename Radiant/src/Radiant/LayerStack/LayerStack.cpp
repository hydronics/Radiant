#include "rdpch.h"
#include "LayerStack.h"

namespace Radiant {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
		m_layer_insert_index++;
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto found = std::find(begin(), end(), layer);
		if (found != end())
		{
			m_layers.erase(found);
			m_layer_insert_index--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto found = std::find(begin(), end(), overlay);
		if (found != end())
		{
			m_layers.erase(found);
		}
	}

}
