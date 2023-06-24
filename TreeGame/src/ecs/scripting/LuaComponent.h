#pragma once

#include "../Components.h"

struct lua_State;

int l_screenToWorld(lua_State* L);
int l_worldToScreen(lua_State* L);
int l_screenToPhys(lua_State* L);
int l_physToScreen(lua_State* L);

int l_scSetVar(lua_State* L);
int l_scGetVar(lua_State* L);

int l_tfGetPosition(lua_State* L);
int l_tfSetPosition(lua_State* L);
int l_tfGetRotation(lua_State* L);
int l_tfSetRotation(lua_State* L);
int l_tfGetScale(lua_State* L);
int l_tfSetScale(lua_State* L);
int l_tfMove(lua_State* L);
int l_tfRotate(lua_State* L);
int l_tfScale(lua_State* L);

int l_rbSetVelocity(lua_State* L);
int l_rbSetSpin(lua_State* L);
int l_rbApplyForce(lua_State* L);
int l_rbApplyTorque(lua_State* L);
int l_rbSetEnabled(lua_State* L);

int l_clGetID(lua_State* L);
int l_clTestPoint(lua_State* L);

int l_anSetFrame(lua_State* L);
int l_anSetAnimation(lua_State* L);
int l_anSetFrameRate(lua_State* L);