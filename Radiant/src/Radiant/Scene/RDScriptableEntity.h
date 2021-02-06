#pragma once

#include "RDEntity.h"

namespace Radiant
{

	class RDScriptableEntity
	{
	public:
		virtual ~RDScriptableEntity() {}

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
		RDEntity entity;
		friend class RDScene;
	};

}
