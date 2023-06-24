#include "PCH.h"
#include "LuaInit.h"

#include "LuaInput.h"
#include "LuaEntity.h"
#include "LuaComponent.h"


static void lua_registerfield(lua_State* L, const char* name, const char* mod, lua_CFunction func)
{
	lua_getglobal(L, mod);

	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_settable(L, -3);

	lua_setglobal(L, mod);
}

static bool lua_hasfunction(lua_State* L, const char* name)
{
	lua_getglobal(L, name);

	if (lua_isfunction(L, -1))
	{
		lua_pop(L, 1);
		return true;
	}

	lua_pop(L, 1);
	return false;
}

static void lua_initcoroutine(lua_State* L, const char* name, ScriptComponent::AsyncFunc& as)
{
	as.thread = lua_newthread(L);
	lua_getglobal(L, "OnMouseClickAsync");
	lua_xmove(L, as.thread, 1);
}


void lua_registerEngineFuncs(lua_State* L)
{
	lua_register(L, "FindEntityByTag",	&l_findEntityByTag);
	lua_register(L, "ScreenToWorld",	&l_screenToWorld);
	lua_register(L, "WorldToScreen",	&l_worldToScreen);
	lua_register(L, "ScreenToPhys",		&l_screenToPhys);
	lua_register(L, "PhysToScreen",		&l_physToScreen);

	lua_registerfield(L, "QueryKey",	"Input",	&l_queryKey);
	lua_registerfield(L, "QueryMouse",	"Input",	&l_queryMouse);
}

void lua_setOverridenFuncs(lua_State* L, uint32_t& funcs)
{
	if (lua_hasfunction(L, "OnHit"))
		funcs |= ScriptComponent::OnHit;
	if (lua_hasfunction(L, "OnExit"))
		funcs |= ScriptComponent::OnExit;

	if (lua_hasfunction(L, "OnMouseClick"))
		funcs |= ScriptComponent::OnMouseClick;
	if (lua_hasfunction(L, "OnMouseMove"))
		funcs |= ScriptComponent::OnMouseMove;
	if (lua_hasfunction(L, "OnMouseRelease"))
		funcs |= ScriptComponent::OnMouseRelease;
	if (lua_hasfunction(L, "OnMouseScroll"))
		funcs |= ScriptComponent::OnMouseScroll;

	if (lua_hasfunction(L, "OnKeyPress"))
		funcs |= ScriptComponent::OnKeyPress;
	if (lua_hasfunction(L, "OnKeyRelease"))
		funcs |= ScriptComponent::OnKeyRelease;

	if (lua_hasfunction(L, "OnCreate"))
		funcs |= ScriptComponent::OnCreate;
	if (lua_hasfunction(L, "OnDestroy"))
		funcs |= ScriptComponent::OnDestroy;

	if (lua_hasfunction(L, "OnUpdate"))
		funcs |= ScriptComponent::OnUpdate;
}

void lua_setCoroutines(lua_State* L, std::unordered_map<ScriptComponent::Func, ScriptComponent::AsyncFunc>& coroutines)
{
	if (lua_hasfunction(L, "OnHitAsync"))
		lua_initcoroutine(L, "OnHitAsync", coroutines[ScriptComponent::OnHit]);
	if (lua_hasfunction(L, "OnExitAsync"))
		lua_initcoroutine(L, "OnExitAsync", coroutines[ScriptComponent::OnExit]);

	if (lua_hasfunction(L, "OnMouseClickAsync"))
		lua_initcoroutine(L, "OnMouseClickAsync", coroutines[ScriptComponent::OnMouseClick]);
	if (lua_hasfunction(L, "OnMouseMoveAsync"))
		lua_initcoroutine(L, "OnMouseMoveAsync", coroutines[ScriptComponent::OnMouseMove]);
	if (lua_hasfunction(L, "OnMouseReleaseAsync"))
		lua_initcoroutine(L, "OnMouseReleaseAsync", coroutines[ScriptComponent::OnMouseRelease]);
	if (lua_hasfunction(L, "OnMouseScrollAsync"))
		lua_initcoroutine(L, "OnMouseScrollAsync", coroutines[ScriptComponent::OnMouseScroll]);

	if (lua_hasfunction(L, "OnKeyPressAsync"))
		lua_initcoroutine(L, "OnKeyPressAsync", coroutines[ScriptComponent::OnKeyPress]);
	if (lua_hasfunction(L, "OnKeyReleaseAsync"))
		lua_initcoroutine(L, "OnKeyReleaseAsync", coroutines[ScriptComponent::OnKeyRelease]);

	if (lua_hasfunction(L, "OnCreateAsync"))
		lua_initcoroutine(L, "OnCreateAsync", coroutines[ScriptComponent::OnCreate]);
	if (lua_hasfunction(L, "OnUpdateAsync"))
		lua_initcoroutine(L, "OnUpdateAsync", coroutines[ScriptComponent::OnUpdate]);
	if (lua_hasfunction(L, "OnDestroyAsync"))
		lua_initcoroutine(L, "OnDestroyAsync", coroutines[ScriptComponent::OnDestroy]);
}