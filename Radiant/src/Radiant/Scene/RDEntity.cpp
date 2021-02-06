#include "rdpch.h"
#include "RDEntity.h"

namespace Radiant {

	RDEntity::RDEntity(entt::entity entity, RDScene* scene)
		: m_entity_handle(entity), m_scene(scene)
	{
	}

}
