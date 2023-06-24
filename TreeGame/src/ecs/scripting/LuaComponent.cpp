#include "PCH.h"
#include "LuaComponent.h"

#include "core/Scene.h"
#include "../Components.h"
#include "LuaConvert.h"


int l_screenToWorld(lua_State* L)
{
	const Scene* scene = static_cast<Scene*>(lua_touserdata(L, 1));
	const sf::Vector2i pos = lua_tovec2i(L, 2);

	lua_pushvec2f(L, scene->screenToWorld(pos));

	return 1;
}

int l_worldToScreen(lua_State* L)
{
	const Scene* scene = static_cast<Scene*>(lua_touserdata(L, 1));
	const sf::Vector2f pos = lua_tovec2f(L, 2);

	lua_pushvec2i(L, scene->worldToScreen(pos));

	return 1;
}

int l_screenToPhys(lua_State* L)
{
	const Scene* scene = static_cast<Scene*>(lua_touserdata(L, 1));
	const sf::Vector2i pos = lua_tovec2i(L, 2);

	lua_pushvec2f(L, scene->screenToPhys(pos));

	return 1;
}

int l_physToScreen(lua_State* L)
{
	const Scene* scene = static_cast<Scene*>(lua_touserdata(L, 1));
	const sf::Vector2f pos = lua_tovec2f(L, 2);

	lua_pushvec2i(L, scene->physToScreen(pos));

	return 1;
}


int l_scSetVar(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	lua_State* scL = static_cast<lua_State*>(lua_touserdata(L, lua_gettop(L)));
	const char* name = lua_tostring(L, 2);
	const int type = static_cast<int>(lua_tointeger(L, 3));

	switch (type)
	{
	case 0:
		lua_pushnumber(scL, lua_tonumber(L, 4));
		break;
	case 1:
		lua_pushboolean(scL, lua_toboolean(L, 4));
		break;
	case 2:
		lua_pushstring(scL, lua_tostring(L, 4));
		break;
	case 3: case 4:
		lua_pushlightuserdata(scL, lua_touserdata(L, 4));
		break;
	}

	lua_setglobal(scL, name);

	return 0;
}

int l_scGetVar(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	lua_State* scL = static_cast<lua_State*>(lua_touserdata(L, lua_gettop(L)));
	const char* name = lua_tostring(L, 2);
	const int type = static_cast<int>(lua_tointeger(L, 3));
	lua_getglobal(scL, name);

	switch (type)
	{
	case 0:
		lua_pushnumber(L, lua_tonumber(scL, -1));
		break;
	case 1:
		lua_pushboolean(L, lua_toboolean(scL, -1));
		break;
	case 2:
		lua_pushstring(L, lua_tostring(scL, -1));
		break;
	case 3: case 4:
		lua_pushlightuserdata(L, lua_touserdata(L, -1));
		break;
	}

	return 1;
}


int l_tfGetPosition(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));

	lua_pushvec2f(L, tf->getPosition());
	
	return 1;
}

int l_tfSetPosition(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const sf::Vector2f pos = lua_tovec2f(L, 2);

	tf->setPosition(pos);

	return 0;
}

int l_tfGetRotation(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));

	lua_pushnumber(L, tf->getRotation());

	return 1;
}

int l_tfSetRotation(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const float rot = static_cast<float>(lua_tonumber(L, 2));

	tf->setRotation(rot);

	return 0;
}

int l_tfGetScale(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));

	lua_pushvec2f(L, tf->getScale());

	return 1;
}

int l_tfSetScale(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const sf::Vector2f scl = lua_tovec2f(L, 2);

	tf->setScale(scl);

	return 0;
}

int l_tfMove(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const sf::Vector2f vec = lua_tovec2f(L, 2);

	tf->move(vec);

	return 0;
}

int l_tfRotate(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const float rot = static_cast<float>(lua_tonumber(L, 2));

	tf->rotate(rot);

	return 0;
}

int l_tfScale(lua_State* L)
{
	auto tf = static_cast<sf::Transformable*>(lua_touserdata(L, 1));
	const sf::Vector2f scl = lua_tovec2f(L, 2);

	tf->scale(scl);

	return 0;
}



int l_rbSetVelocity(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto rb = static_cast<b2Body*>(lua_touserdata(L, lua_gettop(L)));
	const sf::Vector2f vec = lua_tovec2f(L, 2);

	rb->SetLinearVelocity(b2Vec2(vec.x, vec.y));

	return 0;
}

int l_rbSetSpin(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto rb = static_cast<b2Body*>(lua_touserdata(L, lua_gettop(L)));
	const float spin = static_cast<float>(lua_tonumber(L, 2));

	rb->SetAngularVelocity(spin);

	return 0;
}

int l_rbApplyForce(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto rb = static_cast<b2Body*>(lua_touserdata(L, lua_gettop(L)));
	const sf::Vector2f vec = lua_tovec2f(L, 2);

	rb->ApplyForceToCenter(b2Vec2(vec.x, vec.y), true);

	return 0;
}

int l_rbApplyTorque(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto rb = static_cast<b2Body*>(lua_touserdata(L, lua_gettop(L)));
	const float spin = static_cast<float>(lua_tonumber(L, 2));

	rb->ApplyTorque(spin, true);

	return 0;
}

int l_rbSetEnabled(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto rb = static_cast<b2Body*>(lua_touserdata(L, lua_gettop(L)));
	const bool enable = static_cast<bool>(lua_toboolean(L, 2));

	rb->SetEnabled(enable);

	return 0;
}


int l_clGetID(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	b2Fixture* cl = static_cast<b2Fixture*>(lua_touserdata(L, lua_gettop(L)));

	if (const char* str = (const char*)cl->GetUserData().pointer)
		lua_pushstring(L, str);
	else
		lua_pushnil(L);

	return 1;
}

int l_clTestPoint(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	const b2Fixture* cl = static_cast<b2Fixture*>(lua_touserdata(L, lua_gettop(L)));
	const sf::Vector2f p = lua_tovec2f(L, 2);

	lua_pushboolean(L, cl->TestPoint(b2Vec2(p.x, p.y)));

	return 1;
}


int l_anSetFrame(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto an = static_cast<AnimationComponent::_Animation*>(lua_touserdata(L, lua_gettop(L)));
	const uint32_t frame = static_cast<uint32_t>(lua_tointeger(L, 2));

	an->frame = frame;
	an->currenttime = an->frametime;

	return 0;
}

int l_anSetAnimation(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto an = static_cast<AnimationComponent::_Animation*>(lua_touserdata(L, lua_gettop(L)));
	const uint32_t anim = static_cast<uint32_t>(lua_tointeger(L, 2));

	an->anim = anim;
	an->currenttime = an->frametime;

	return 0;
}

int l_anSetFrameRate(lua_State* L)
{
	lua_pushstring(L, "obj");
	lua_gettable(L, 1);
	auto an = static_cast<AnimationComponent::_Animation*>(lua_touserdata(L, lua_gettop(L)));
	const uint32_t framerate = static_cast<uint32_t>(lua_tointeger(L, 2));

	an->frametime = 1.f / framerate;

	return 0;
}