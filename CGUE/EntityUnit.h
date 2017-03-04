#pragma once
namespace Engine {
	class Entity;

	class EntityUnit
	{
		friend Entity;

		Entity *entity;
	public:
		EntityUnit();
		virtual ~EntityUnit();

		Entity* EntityUnit::getEntity() const;

		virtual void Init();
		virtual void Wire();
		const char *GetClassName() const;
	};
}

