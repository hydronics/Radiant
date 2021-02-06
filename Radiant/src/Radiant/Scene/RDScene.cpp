#include "rdpch.h"

#include "RDScene.h"
#include "RDEntity.h"
#include "Components.h"

#include "Radiant/Renderer/Renderer2d.h"

namespace Radiant {

	RDScene::RDScene()
	{
	}

	RDScene::~RDScene()
	{

	}

	void RDScene::OnUpdate(Timestep ts)
	{
		// Update the native scripts
		{
			Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.CreateInstanceFunction();
						nsc.Instance->entity = RDEntity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render the entire scene
		// Get the main camera for rendering.
		// @todo: abstract this to general GetPrimaryCameras() helper
		RDCamera* main_camera = nullptr;
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

	void RDScene::OnViewportResize(uint32_t width, uint32_t height)
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

	RDEntity RDScene::CreateEntity(const std::string& name)
	{
		RDEntity entity = { Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void RDScene::DestroyEntity(RDEntity entity)
	{
		Registry.destroy(entity);
	}

	template<typename T>
	void RDScene::OnComponentAdded(RDEntity entity, T& comopnent)
	{
		static_assert(false);
	}

	template<>
	void RDScene::OnComponentAdded<TagComponent>(RDEntity entity, TagComponent& component)
	{

	}

	template<>
	void RDScene::OnComponentAdded<TransformComponent>(RDEntity entity, TransformComponent& component)
	{

	}
	template<>
	void RDScene::OnComponentAdded<CameraComponent>(RDEntity entity, CameraComponent& component)
	{
		component.Camera.SetViewportDimensions(ViewportWidth, ViewportHeight);
	}
	template<>
	void RDScene::OnComponentAdded<SpriteComponent>(RDEntity entity, SpriteComponent& component)
	{

	}
	template<>
	void RDScene::OnComponentAdded<NativeScriptComponent>(RDEntity entity, NativeScriptComponent& component)
	{
	}
}
