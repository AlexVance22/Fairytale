#include "PCH.h"
#include "Collision.h"

#include "core/Scene.h"

#include "ecs/Entity.h"
#include "ecs/Components.h"

#include "ecs/scripting/LuaEntity.h"
#include "ecs/scripting/LuaConvert.h"


void ContactListener::BeginContact(b2Contact* contact)
{
	b2Body* a_body = contact->GetFixtureA()->GetBody();
	b2Body* b_body = contact->GetFixtureB()->GetBody();

	Entity a_entity(p_scene, static_cast<entt::entity>(a_body->GetUserData().pointer));
	Entity b_entity(p_scene, static_cast<entt::entity>(b_body->GetUserData().pointer));

	if (auto sc = a_entity.getComponentIfHas<ScriptComponent>())
	{
		if (sc->funcs & ScriptComponent::OnHit)
		{
			lua_getglobal(sc->L, "OnHit");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_call(sc->L, 3, 0);
		}
		auto& asfunc = sc->coroutines[ScriptComponent::OnHit];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc->L, "OnHitAsync");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			int retvalcount = 1;
			if (lua_resume(sc->L, asfunc.thread, 3, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)
				asfunc.delay = static_cast<float>(lua_tonumber(sc->L, -1));
		}
	}

	if (auto sc = b_entity.getComponentIfHas<ScriptComponent>())
	{
		if (sc->funcs & ScriptComponent::OnHit)
		{
			lua_getglobal(sc->L, "OnHit");
			lua_pushentity(sc->L, a_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_call(sc->L, 3, 0);
		}
		auto& asfunc = sc->coroutines[ScriptComponent::OnHit];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc->L, "OnHitAsync");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			int retvalcount = 1;
			if (lua_resume(sc->L, asfunc.thread, 3, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)
				asfunc.delay = static_cast<float>(lua_tonumber(sc->L, -1));
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Body* a_body = contact->GetFixtureA()->GetBody();
	b2Body* b_body = contact->GetFixtureB()->GetBody();

	Entity a_entity(p_scene, static_cast<entt::entity>(a_body->GetUserData().pointer));
	Entity b_entity(p_scene, static_cast<entt::entity>(b_body->GetUserData().pointer));

	if (auto sc = a_entity.getComponentIfHas<ScriptComponent>())
	{
		if (sc->funcs & ScriptComponent::OnExit)
		{
			lua_getglobal(sc->L, "OnExit");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_call(sc->L, 3, 0);
		}
		auto& asfunc = sc->coroutines[ScriptComponent::OnExit];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc->L, "OnExitAsync");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			int retvalcount = 1;
			if (lua_resume(sc->L, asfunc.thread, 3, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)
				asfunc.delay = static_cast<float>(lua_tonumber(sc->L, -1));
		}
	}

	if (auto sc = b_entity.getComponentIfHas<ScriptComponent>())
	{
		if (sc->funcs & ScriptComponent::OnExit)
		{
			lua_getglobal(sc->L, "OnExit");
			lua_pushentity(sc->L, a_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			lua_call(sc->L, 3, 0);
		}
		auto& asfunc = sc->coroutines[ScriptComponent::OnExit];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc->L, "OnExitAsync");
			lua_pushentity(sc->L, b_entity);
			lua_pushcolliderRef(sc->L, contact->GetFixtureB());
			lua_pushcolliderRef(sc->L, contact->GetFixtureA());
			int retvalcount = 1;
			if (lua_resume(sc->L, asfunc.thread, 3, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)
				asfunc.delay = static_cast<float>(lua_tonumber(sc->L, -1));
		}
	}
}


ContactListener::ContactListener(Scene* scene)
	: p_scene(scene)
{}