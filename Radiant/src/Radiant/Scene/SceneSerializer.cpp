#include "rdpch.h"

#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>

namespace Radiant {


	void SerializeEntity(YAML::Emitter emitter, RDEntity entity)
	{

	}

	bool DeserializeEntity(RDEntity entity)
	{
		return false;
	}

	SceneSerializer::SceneSerializer(const Ref<RDScene>& scene)
		: SceneContext(scene)
	{
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		using namespace YAML;
		Emitter out;
		out << BeginMap;
		out << Key << "Scene";
		out << YAML::Value << SceneContext->Name; //@TODO: Give Scene's name fields.
		out << Key << "Entities";
		out << YAML::Value << BeginSeq;
		SceneContext->Registry.each[&](auto entityID)
		{
			Entity entity = { entityID, SceneContext.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << EndSeq;
		out << EndMap;

		std::ofstream outsave(filepath);
		outsave << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		RD_CORE_ASSERT(false, "Binary serialization not yet implemented.");
	}

	bool SceneSerializer::DeserializeText(const std::string& filepath)
	{
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		RD_CORE_ASSERT(false, "Binary deserialization not yet implemented.");
		return false;
	}

}
