#include "PCH.h"
#include "LuaConvert.h"

#include "../Entity.h"
#include "LuaComponent.h"

class Scene;


void lua_pushvec2f(lua_State* L, const sf::Vector2f& vec)
{
	lua_newtable(L);

	lua_pushstring(L, "x");
	lua_pushnumber(L, vec.x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, vec.y);
	lua_settable(L, -3);
}

void lua_pushvec2i(lua_State* L, const sf::Vector2i& vec)
{
	lua_newtable(L);

	lua_pushstring(L, "x");
	lua_pushnumber(L, vec.x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, vec.y);
	lua_settable(L, -3);
}

void lua_pushvec2u(lua_State* L, const sf::Vector2u& vec)
{
	lua_newtable(L);

	lua_pushstring(L, "x");
	lua_pushnumber(L, vec.x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, vec.y);
	lua_settable(L, -3);
}

void lua_pushcolor(lua_State* L, const sf::Color& color)
{
	lua_newtable(L);

	lua_pushstring(L, "r");
	lua_pushnumber(L, color.r);
	lua_settable(L, -3);

	lua_pushstring(L, "g");
	lua_pushnumber(L, color.g);
	lua_settable(L, -3);

	lua_pushstring(L, "b");
	lua_pushnumber(L, color.b);
	lua_settable(L, -3);

	lua_pushstring(L, "a");
	lua_pushnumber(L, color.a);
	lua_settable(L, -3);
}

void lua_pushvertex(lua_State* L, const sf::Vertex& vert)
{
	lua_newtable(L);

	lua_pushstring(L, "position");
	lua_pushvec2f(L, vert.position);
	lua_settable(L, -3);

	lua_pushstring(L, "color");
	lua_pushcolor(L, vert.color);
	lua_settable(L, -3);

	lua_pushstring(L, "texcoord");
	lua_pushvec2f(L, vert.texCoords);
	lua_settable(L, -3);
}


sf::Vector2f lua_tovec2f(lua_State* L, int idx)
{
	lua_pushstring(L, "x");
	lua_gettable(L, idx);
	const float x = static_cast<float>(lua_tonumber(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "y");
	lua_gettable(L, idx);
	const float y = static_cast<float>(lua_tonumber(L, -1));
	lua_pop(L, 1);

	return sf::Vector2f(x, y);
}

sf::Vector2i lua_tovec2i(lua_State* L, int idx)
{
	lua_pushstring(L, "x");
	lua_gettable(L, idx);
	const int x = static_cast<int>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "y");
	lua_gettable(L, idx);
	const int y = static_cast<int>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	return sf::Vector2i(x, y);
}

sf::Vector2u lua_tovec2u(lua_State* L, int idx)
{
	lua_pushstring(L, "x");
	lua_gettable(L, idx);
	const uint32_t x = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "y");
	lua_gettable(L, idx);
	const uint32_t y = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	return sf::Vector2u(x, y);
}

sf::Color lua_tocolor(lua_State* L, int idx)
{
	lua_pushstring(L, "r");
	lua_gettable(L, idx);
	const uint32_t r = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "g");
	lua_gettable(L, idx);
	const uint32_t g = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "b");
	lua_gettable(L, idx);
	const uint32_t b = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "a");
	lua_gettable(L, idx);
	const uint32_t a = static_cast<uint32_t>(lua_tointeger(L, -1));
	lua_pop(L, 1);

	return sf::Color(r, g, b, a);
}

sf::Vertex lua_tovertex(lua_State* L, int idx)
{
	lua_pushstring(L, "position");
	lua_gettable(L, idx);
	const sf::Vector2f pos = lua_tovec2f(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "color");
	lua_gettable(L, idx);
	const sf::Color col = lua_tocolor(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "texcoord");
	lua_gettable(L, idx);
	const sf::Vector2f tex = lua_tovec2f(L, -1);
	lua_pop(L, 1);

	return sf::Vertex(pos, col, tex);
}


void lua_pushtransformRef(lua_State* L, sf::Transformable& tf)
{
	lua_newtable(L);

	lua_pushstring(L, "GetPosition");
	lua_pushcfunction(L, &l_tfGetPosition);
	lua_settable(L, -3);

	lua_pushstring(L, "SetPosition");
	lua_pushcfunction(L, &l_tfSetPosition);
	lua_settable(L, -3);

	lua_pushstring(L, "GetRotation");
	lua_pushcfunction(L, &l_tfGetRotation);
	lua_settable(L, -3);

	lua_pushstring(L, "SetRotation");
	lua_pushcfunction(L, &l_tfSetRotation);
	lua_settable(L, -3);

	lua_pushstring(L, "GetScale");
	lua_pushcfunction(L, &l_tfGetScale);
	lua_settable(L, -3);

	lua_pushstring(L, "SetScale");
	lua_pushcfunction(L, &l_tfSetScale);
	lua_settable(L, -3);

	lua_pushstring(L, "Move");
	lua_pushcfunction(L, &l_tfMove);
	lua_settable(L, -3);

	lua_pushstring(L, "Rotate");
	lua_pushcfunction(L, &l_tfRotate);
	lua_settable(L, -3);

	lua_pushstring(L, "Scale");
	lua_pushcfunction(L, &l_tfScale);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_pushlightuserdata(L, &tf);
	lua_settable(L, -3);
}

void lua_pushvertexarrayRef(lua_State* L, sf::VertexArray& va)
{

}

void lua_pushmaterialRef(lua_State* L, MaterialComponent& mat)
{
	lua_newtable(L);

	lua_pushstring(L, "texture");
	lua_pushlightuserdata(L, mat.texture);
	lua_settable(L, -3);

	lua_pushstring(L, "shader");
	lua_pushlightuserdata(L, mat.shader);
	lua_settable(L, -3);
}

void lua_pushanimationRef(lua_State* L, AnimationComponent& anim)
{
	lua_newtable(L);

	lua_pushstring(L, "SetFrame");
	lua_pushcfunction(L, &l_anSetFrame);
	lua_settable(L, -3);

	lua_pushstring(L, "SetAnimation");
	lua_pushcfunction(L, &l_anSetAnimation);
	lua_settable(L, -3);

	lua_pushstring(L, "SetFrameRate");
	lua_pushcfunction(L, &l_anSetFrameRate);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_pushlightuserdata(L, anim.animation.get());
	lua_settable(L, -3);
}

void lua_pushrigidbodyRef(lua_State* L, b2Body* body)
{
	lua_newtable(L);

	lua_pushstring(L, "SetVelocity");
	lua_pushcfunction(L, &l_rbSetVelocity);
	lua_settable(L, -3);

	lua_pushstring(L, "SetSpin");
	lua_pushcfunction(L, &l_rbSetSpin);
	lua_settable(L, -3);

	lua_pushstring(L, "ApplyForce");
	lua_pushcfunction(L, &l_rbApplyForce);
	lua_settable(L, -3);

	lua_pushstring(L, "ApplyTorque");
	lua_pushcfunction(L, &l_rbApplyTorque);
	lua_settable(L, -3);

	lua_pushstring(L, "SetEnabled");
	lua_pushcfunction(L, &l_rbSetEnabled);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_pushlightuserdata(L, body);
	lua_settable(L, -3);
}

void lua_pushcolliderRef(lua_State* L, b2Fixture* fix)
{
	lua_newtable(L);

	lua_pushstring(L, "GetID");
	lua_pushcfunction(L, &l_clGetID);
	lua_settable(L, -3);

	lua_pushstring(L, "TestPoint");
	lua_pushcfunction(L, &l_clTestPoint);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_pushlightuserdata(L, fix);
	lua_settable(L, -3);
}

void lua_pushscriptRef(lua_State* L, lua_State* script)
{
	lua_newtable(L);

	lua_pushstring(L, "SetVar");
	lua_pushcfunction(L, &l_scSetVar);
	lua_settable(L, -3);

	lua_pushstring(L, "GetVar");
	lua_pushcfunction(L, &l_scGetVar);
	lua_settable(L, -3);

	lua_pushstring(L, "obj");
	lua_pushlightuserdata(L, script);
	lua_settable(L, -3);
}


void lua_pushtransformVal(lua_State* L, const sf::Transformable& tf)
{
	lua_newtable(L);

	lua_pushstring(L, "loc");
	lua_pushvec2f(L, tf.getPosition());
	lua_settable(L, -3);

	lua_pushstring(L, "rot");
	lua_pushnumber(L, tf.getRotation());
	lua_settable(L, -3);

	lua_pushstring(L, "scl");
	lua_pushvec2f(L, tf.getScale());
	lua_settable(L, -3);
}

void lua_pushvertexarrayVal(lua_State* L, const sf::VertexArray& va)
{
	lua_newtable(L);

	lua_pushstring(L, "primitive");
	lua_pushnumber(L, static_cast<lua_Number>(va.getVertexCount()));
	lua_settable(L, -3);

	lua_pushstring(L, "vertices");
	lua_newtable(L);
	for (size_t i = 0; i < va.getVertexCount(); i++)
	{
		lua_pushvertex(L, va[i]);
		lua_settable(L, -3);
	}
	lua_settable(L, -3);
}

void lua_pushmaterialVal(lua_State* L, MaterialComponent& mat)
{
	lua_pushmaterialRef(L, mat);
}

void lua_pushanimationVal(lua_State* L, const AnimationComponent::_Animation& anim)
{
	lua_newtable(L);

	lua_pushstring(L, "cellcount");
	lua_pushvec2u(L, sf::Vector2u(anim.framecount, anim.animcount));
	lua_settable(L, -3);

	lua_pushstring(L, "cellsize");
	lua_pushvec2u(L, anim.cellsize);
	lua_settable(L, -3);

	lua_pushstring(L, "currentcell");
	lua_pushvec2u(L, sf::Vector2u(anim.frame, anim.anim));
	lua_settable(L, -3);

	lua_pushstring(L, "framerate");
	lua_pushnumber(L, 1.f / anim.frametime);
	lua_settable(L, -3);

	lua_pushstring(L, "playing");
	lua_pushboolean(L, anim.playing);
	lua_settable(L, -3);

	lua_pushstring(L, "looping");
	lua_pushboolean(L, anim.loop);
	lua_settable(L, -3);
}