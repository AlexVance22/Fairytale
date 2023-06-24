#pragma once

#include "core/Scene.h"


using EntityError = std::exception;


class Entity
{
private:
	entt::entity m_id = entt::null;
	Scene* p_scene = nullptr;

public:
	Entity(Scene* scene, entt::entity id);
	Entity(Scene* scene);

	template<typename Comp>
	bool hasComponent()
	{
		return p_scene->m_registry.has<Comp>(m_id);
	}

	template<typename Comp, typename ...Args>
	Comp& addComponent(Args&&... args)
	{
		if (hasComponent<Comp>())
			throw EntityError("Entity already has component");

		return p_scene->m_registry.emplace<Comp>(m_id, std::forward<Args>(args)...);
	}

	template<typename Comp>
	void removeComponent()
	{
		if (!hasComponent<Comp>())
			throw EntityError("Entity does not have component");

		p_scene->m_registry.remove<Comp>(m_id);
	}

	template<typename Comp>
	void removeComponentIfHas()
	{
		p_scene->m_registry.remove_if_exists<Comp>(m_id);
	}

	template<typename Comp>
	Comp& getComponent()
	{
		if (!hasComponent<Comp>())
			throw EntityError("Entity does not have component");

		return p_scene->m_registry.get<Comp>(m_id);
	}

	template<typename Comp>
	const Comp& getComponent() const
	{
		if (!hasComponent<Comp>())
			throw EntityError("Entity does not have component");

		return p_scene->m_registry.get<Comp>(m_id);
	}

	template<typename Comp>
	Comp* getComponentIfHas()
	{
		return p_scene->m_registry.try_get<Comp>(m_id);
	}

	template<typename Comp>
	const Comp* getComponentIfHas() const
	{
		return p_scene->m_registry.try_get<Comp>(m_id);
	}

	entt::entity getEnttID() { return m_id; }
	Scene* getScene() { return p_scene; }

	void destroy();
};