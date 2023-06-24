#include "PCH.h"
#include "Entity.h"

#include "Components.h"


Entity::Entity(Scene* scene, entt::entity id)
	: p_scene(scene), m_id(id)
{}

Entity::Entity(Scene* scene)
	: p_scene(scene)
{
	m_id = p_scene->m_registry.create();
}


void Entity::destroy()
{
	if (!hasComponent<DestroyComponent>())
		addComponent<DestroyComponent>();

	p_scene = nullptr;
	m_id = entt::null;
}