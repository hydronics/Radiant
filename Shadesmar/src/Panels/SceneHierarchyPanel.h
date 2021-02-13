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
		SceneHierarchyPanel(const Ref<Scene>& Scene);

		void SetContext(const Ref<Scene>& Scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return SelectedEntityContext; }

	private:
		void DrawEntityUINode(Entity EntityContext);		
		void DrawEntityComponents(Entity EntityContext);

	private:
		Ref<Scene> SceneContext;
		Entity SelectedEntityContext;
	};

}