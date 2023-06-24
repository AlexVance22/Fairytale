#include "PCH.h"
#include "LuaEntity.h"

#include "../Entity.h"
#include "../Components.h"

#include "LuaConvert.h"


int l_getComponentVal(lua_State* L)
{
	Entity entity = lua_toentity(L, 1);
	const uint32_t comptype = static_cast<uint32_t>(lua_tointeger(L, 2));

	switch (comptype)
	{
	case 0:
		lua_pushstring(L, entity.getComponent<TagComponent>().tag.c_str());
		break;
	case 1:
		lua_pushtransformVal(L, entity.getComponent<TransformComponent>().transform);
		break;
	case 2:
		lua_pushstring(L, entity.getComponent<LayerComponent>().layer.c_str());
		break;
	case 3:
		lua_pushvertexarrayVal(L, entity.getComponent<VertexArrayComponent>().vertexarray);
		break;
	case 4:
		lua_pushmaterialVal(L, entity.getComponent<MaterialComponent>());
		break;
	case 5:
		lua_pushanimationVal(L, *entity.getComponent<AnimationComponent>().animation);
		break;
	case 6:
		//Movement
		break;
	case 7:
		lua_pushlightuserdata(L, entity.getComponent<RigidBodyComponent>().body);
		break;
	case 8:
		lua_pushlightuserdata(L, entity.getComponent<ColliderComponent>().fixture);
		break;
	case 9:
		lua_pushlightuserdata(L, entity.getComponent<ScriptComponent>().L);
		break;
	}

	return 1;
}

int l_getComponentRef(lua_State* L)
{
	Entity entity = lua_toentity(L, 1);
	const uint32_t comptype = static_cast<uint32_t>(lua_tointeger(L, 2));

	switch (comptype)
	{
	case 0:
		lua_pushlightuserdata(L, entity.getComponent<TagComponent>().tag.data());
		break;
	case 1:
		lua_pushtransformRef(L, entity.getComponent<TransformComponent>().transform);
		break;
	case 2:
		lua_pushlightuserdata(L, entity.getComponent<LayerComponent>().layer.data());
		break;
	case 3:
		lua_pushlightuserdata(L, &entity.getComponent<VertexArrayComponent>().vertexarray[0]);
		break;
	case 4:
		lua_pushmaterialRef(L, entity.getComponent<MaterialComponent>());
		break;
	case 5:
		lua_pushanimationRef(L, entity.getComponent<AnimationComponent>());
		break;
	case 6:
		lua_pushlightuserdata(L, &entity.getComponent<MovementComponent>());
		break;
	case 7:
		lua_pushrigidbodyRef(L, entity.getComponent<RigidBodyComponent>().body);
		break;
	case 8:
		lua_pushcolliderRef(L, entity.getComponent<ColliderComponent>().fixture);
		break;
	case 9:
		lua_pushscriptRef(L, entity.getComponent<ScriptComponent>().L);
		break;
	}

	return 1;
}

int l_hasComponent(lua_State* L)
{
	Entity entity = lua_toentity(L, 1);
	const uint32_t comptype = static_cast<uint32_t>(lua_tointeger(L, 2));

	switch (comptype)
	{
	case 0: case 1: case 2:
		lua_pushboolean(L, true);
		break;
	case 3:
		lua_pushboolean(L, entity.hasComponent<VertexArrayComponent>());
		break;
	case 4:
		lua_pushboolean(L, entity.hasComponent<MaterialComponent>());
		break;
	case 5:
		lua_pushboolean(L, entity.hasComponent<AnimationComponent>());
		break;
	case 6:
		lua_pushboolean(L, entity.hasComponent<MovementComponent>());
		break;
	case 7:
		lua_pushboolean(L, entity.hasComponent<RigidBodyComponent>());
		break;
	case 8:
		lua_pushboolean(L, entity.hasComponent<ColliderComponent>());
		break;
	case 9:
		lua_pushboolean(L, entity.hasComponent<ScriptComponent>());
		break;

	}

	return 1;
}

int l_destroy(lua_State* L)
{
	lua_toentity(L, 1).destroy();

	return 0;
}


int l_findEntityByTag(lua_State* L)
{
	Scene* scene = static_cast<Scene*>(lua_touserdata(L, 1));
	const char* tag = lua_tostring(L, 2);

	lua_pushentity(L, scene->findEntityByTag(tag));

	return 1;
}


void lua_pushentity(lua_State* L, Entity e)
{
	lua_newtable(L);

	lua_pushstring(L, "GetComponentRef");
	lua_pushcfunction(L, &l_getComponentRef);
	lua_settable(L, -3);

	lua_pushstring(L, "GetComponentVal");
	lua_pushcfunction(L, &l_getComponentVal);
	lua_settable(L, -3);

	lua_pushstring(L, "HasComponent");
	lua_pushcfunction(L, &l_hasComponent);
	lua_settable(L, -3);

	lua_pushstring(L, "Destroy");
	lua_pushcfunction(L, &l_destroy);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_newtable(L);

	lua_pushstring(L, "scene");
	lua_pushlightuserdata(L, e.getScene());
	lua_settable(L, -3);

	lua_pushstring(L, "id");
	lua_pushnumber(L, static_cast<lua_Number>(e.getEnttID()));
	lua_settable(L, -3);

	lua_settable(L, -3);
}

Entity lua_toentity(lua_State* L, int idx)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, idx);

	lua_pushstring(L, "scene");
	lua_gettable(L, -2);
	Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "id");
	lua_gettable(L, -2);
	entt::entity id = static_cast<entt::entity>(lua_tonumber(L, -1));
	lua_pop(L, 1);

	return Entity(scene, id);
}