#pragma once

#include "Radiant/Core/Core.h"
#include "Radiant/Core/Log.h"
#include "Radiant/Scene/RDScene.h"
#include "Radiant/Scene/RDEntity.h"

namespace Radiant {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<RDScene>& SceneContext);

		void SetContext(const Ref<RDScene>& SceneContext);

		void OnImGuiRender();

	private:
		void DrawEntityUINode(RDEntity EntityContext);		
		void DrawEntityComponents(RDEntity EntityContext);

	private:
		Ref<RDScene> SceneContext;
		RDEntity SelectedEntityContext;
	};

}