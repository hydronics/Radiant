#include "rdpch.h"
#include "Entity.h"

namespace Radiant {

	Entity::Entity(entt::entity entity, Scene* scene)
		: m_entity_handle(entity), m_scene(scene)
	{
	}

}
