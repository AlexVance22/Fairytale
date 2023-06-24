#pragma once

#include "../Components.h"


void lua_registerEngineFuncs(lua_State* L);

void lua_setOverridenFuncs(lua_State* L, uint32_t& funcs);

void lua_setCoroutines(lua_State* L, std::unordered_map<ScriptComponent::Func, ScriptComponent::AsyncFunc>& coroutines);