#pragma once

#include "Radiant/Core/Core.h"
#include "Radiant/Core/Log.h"
#include "Radiant/Scene/Scene.h"
#include "Radiant/Scene/Entity.h"

namespace Radiant {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityUINode(Entity entity);		
		void DrawEntityComponents(Entity entity);

	private:
		Ref<Scene> SceneContext;
		Entity SelectedEntityContext;
	};

}