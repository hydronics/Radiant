#include "SceneHierarchyPanel.h"
#include "Radiant/Scene/Components.h"

#include <imgui/imgui.h>


namespace Radiant {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{

	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		SceneContext = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		SceneContext->m_registry.each([&](auto entity)
		{
			Entity CurEntity{ entity, SceneContext.get() };
			DrawEntityUINode(CurEntity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityUINode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
		nodeFlags = (SelectedEntityContext == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, nodeFlags, tc.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectedEntityContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntityComponents(Entity entity)
	{

	}

}
