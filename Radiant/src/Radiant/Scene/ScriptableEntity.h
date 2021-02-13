#pragma once

#include "Entity.h"

namespace Radiant
{

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};

	private:
		Entity entity;
		friend class Scene;
	};

}
