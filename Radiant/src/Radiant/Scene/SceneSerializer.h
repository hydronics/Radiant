#pragma once

#include "RDScene.h"

namespace Radiant {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<RDScene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

	private:
		Ref<RDScene> SceneContext;
	};

}