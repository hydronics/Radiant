#include "rdpch.h"

#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
//#include "RDSceneCamera.h"
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& vec)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			return true;
		}
	};


	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();
			return true;
		}
	};
}


namespace Radiant {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273";

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& tc = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Camera";
			out << YAML::BeginMap;

			auto& camera = tc.Camera;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveVerticalFov" << YAML::Value << camera.GetPerspectiveVerticalFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();

			out << YAML::EndMap;
			out << YAML::Key << "Primary" << YAML::Value << tc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << tc.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << tc.Color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	static bool DeserializeEntity(Entity entity)
	{
		return false;
	}

	/// <summary>
	/// SceneSerializer
	/// </summary>
	/// <param name="scene"></param>
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: SceneContext(scene)
	{
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << SceneContext->Name;
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		SceneContext->Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, SceneContext.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream outsave(filepath);
		outsave << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		RD_CORE_ASSERT(false, "Binary serialization not yet implemented.");
	}

	bool SceneSerializer::DeserializeText(const std::string& filepath)
	{
		std::ifstream readFile(filepath);
		std::stringstream strStream;

		strStream << readFile.rdbuf();

		YAML::Node rootNode = YAML::Load(strStream.str().c_str());
		if (!rootNode["Scene"])
		{
			return false;
		}

		std::string sceneName = rootNode["Scene"].as<std::string>();
		RD_CORE_TRACE("Deserializing Scene: '{0}'", sceneName.c_str());

		auto entities = rootNode["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				// ID
				uint64_t entityID = entity["Entity"].as<uint64_t>();

				// Tag comp
				std::string name;
				auto tagNode = entity["TagComponent"];
				if (tagNode)
				{
					name = tagNode["Tag"].as<std::string>();
					RD_CORE_TRACE("Deserializing Entity: '{0}'", name.c_str());
				}

				Entity deserializedEntity = SceneContext->CreateEntity(name);

				// Camera comp
				auto transformNode = entity["TransformComponent"];
				if (transformNode)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformNode["Translation"].as<glm::vec3>();
					tc.Rotation = transformNode["Rotation"].as<glm::vec3>();
					tc.Scale = transformNode["Scale"].as<glm::vec3>();
				}

				// Camera comp
				auto camNode = entity["CameraComponent"];
				if (camNode)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProps = camNode["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFov(cameraProps["PerspectiveVerticalFov"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = camNode["Primary"].as<bool>();
					cc.FixedAspectRatio = camNode["FixedAspectRatio"].as<bool>();
				}
				
				// Sprite comp
				auto spriteNode = entity["SpriteComponent"];
				if (spriteNode)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponent>();
					src.Color = spriteNode["Color"].as<glm::vec4>();
				}

				// Script comp
				auto scriptNode = entity["NativeScriptComponent"];
				if (scriptNode)
				{
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		RD_CORE_ASSERT(false, "Binary deserialization not yet implemented.");
		return false;
	}

}
