#include "rdpch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Radiant/Renderer/Renderer2d.h"

namespace Radiant {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Render the entire scene

		// Get the main camera for rendering.
		// @todo: abstract this to general GetPrimaryCameras() helper
		Camera* main_camera = nullptr;
		glm::mat4* camera_transform = nullptr;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					main_camera = &camera.camera;
					camera_transform = &transform.transform;
					break;
				}
			}
		}
	
		if (main_camera)
		{
			Renderer2d::BeginScene(main_camera->GetProjection(), *camera_transform);

			// Draw all of our sprite components in the scene.
			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);

			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2d::DrawQuad(transform, sprite.color);
			}

			Renderer2d::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewport_width = width;
		m_viewport_height = height;

		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera_comp = view.get<CameraComponent>(entity);
			if (!camera_comp.fixed_aspect_ratio)
			{
				camera_comp.camera.SetViewportDimensions(m_viewport_width, m_viewport_height);
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

}