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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			SelectedEntityContext = {};
		}
		ImGui::End();


		ImGui::Begin("Entity Components");
		if (SelectedEntityContext)
		{
			DrawEntityComponents(SelectedEntityContext);
		}
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
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>().Transform;

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& camera = entity.GetComponent<CameraComponent>().Camera;

			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; ++i)
					{
						// Check which is selected and set the current projection type.
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size))
					{
						camera.SetOrthographicSize(size);
					}

					float nearClip = camera.GetOrthographicNear();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetOrthographicNear(nearClip);
					}

					float farClip = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetOrthographicFar(farClip);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFov());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
					{
						camera.SetPerspectiveVerticalFov(glm::radians(verticalFov));
					}

					float nearClip = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetPerspectiveNear(nearClip);
					}

					float farClip = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetPerspectiveFar(farClip);
					}
				}

				ImGui::TreePop();
			}
		}
	}

}
