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
		// Update the native scripts
		{
			Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.CreateInstanceFunction();
						nsc.Instance->entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render the entire scene
		// Get the main camera for rendering.
		// @todo: abstract this to general GetPrimaryCameras() helper
		BaseCamera* main_camera = nullptr;
		glm::mat4 camera_transform;
		{
			auto view = Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [tc, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					main_camera = &camera.Camera;
					camera_transform = tc.GetTransform();
					break;
				}
			}
		}
	
		if (main_camera)
		{
			Renderer2d::BeginScene(main_camera->GetProjection(), camera_transform);

			// Draw all of our sprite components in the scene.
			auto group = Registry.group<TransformComponent>(entt::get<SpriteComponent>);

			for (auto entity : group)
			{
				auto [tc, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2d::DrawQuad(tc.GetTransform(), sprite.Color);
			}

			Renderer2d::EndScene();
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		ViewportWidth = width;
		ViewportHeight = height;

		auto view = Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera_comp = view.get<CameraComponent>(entity);
			if (!camera_comp.FixedAspectRatio)
			{
				camera_comp.Camera.SetViewportDimensions(ViewportWidth, ViewportHeight);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto camView = Registry.view<CameraComponent>();
		for (auto entity : camView)
		{
			const auto& cc = camView.get<CameraComponent>(entity);
			if (cc.Primary)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		Registry.destroy(entity);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& comopnent)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportDimensions(ViewportWidth, ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}
