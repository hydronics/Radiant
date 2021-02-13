#include "SceneHierarchyPanel.h"
#include "Radiant/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>


namespace Radiant {

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

	// Local template function for abstracting all the same stuff when drawing an entity component
	template<typename T, typename UIFunc>
	void DrawComponent(const std::string& name, Entity entity, UIFunc drawCode)
	{
		const ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			auto& component = entity.GetComponent<T>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool NodeOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TreeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegion.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					removeComponent = true;

				}
				ImGui::EndPopup();
			}

			if (NodeOpen)
			{
				drawCode(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& Scene)
	{

	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& Scene)
	{
		SceneContext = Scene;
		SelectedEntityContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		SceneContext->Registry.each([&](auto entity)
		{
			Entity CurEntity{ entity, SceneContext.get() };
			DrawEntityUINode(CurEntity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			SelectedEntityContext = {};
		}

		// Right clicking on a blank space
		if (ImGui::BeginPopupContextWindow(NULL, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				SceneContext->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End(); // "Scene Hierarchy"

		// Currently selected entity will draw have components drawn in Component View.
		ImGui::Begin("Properties");
		if (SelectedEntityContext)
		{
			DrawEntityComponents(SelectedEntityContext);
		}
		ImGui::End(); // "Properties"
	}

	void SceneHierarchyPanel::DrawEntityUINode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags nodeFlags = (SelectedEntityContext == entity) ? ImGuiTreeNodeFlags_Selected : 0;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, nodeFlags, tc.Tag.c_str());
		bool shouldDestroy = false;

		if (ImGui::IsItemClicked())
		{
			SelectedEntityContext = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				shouldDestroy = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (shouldDestroy)
		{
			SceneContext->DestroyEntity(entity);
			if (SelectedEntityContext == entity)
			{
				SelectedEntityContext = {};
			}
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

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				SelectedEntityContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite"))
			{
				SelectedEntityContext.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform Component", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);

			glm::vec3 rotationInDegrees = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotationInDegrees);
			component.Rotation = glm::radians(rotationInDegrees);

			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		ImGui::PopItemWidth();

		DrawComponent<CameraComponent>("Camera Component", entity, [](auto& component)
		{
			auto& camera = component.Camera;
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			ImGui::Checkbox("Primary", &component.Primary);

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
		});

		DrawComponent<SpriteComponent>("Sprite Component", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
	}


	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X-Coord button, followed by drag float widget
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.85f, 0.15f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.95f, 0.1f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y-Coord button, followed by drag float widget
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.70f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.95f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z-Coord button, followed by drag float widget
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.15f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.1f, 0.95f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

}
