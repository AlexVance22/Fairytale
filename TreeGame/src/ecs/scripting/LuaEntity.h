#pragma once

class Entity;

int l_getComponentVal(lua_State* L);
int l_getComponentRef(lua_State* L);
int l_hasComponent(lua_State* L);
int l_destroy(lua_State* L);

int l_findEntityByTag(lua_State* L);

void lua_pushentity(lua_State* L, Entity e);
Entity lua_toentity(lua_State* L, int idx);