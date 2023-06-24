#pragma once

#include "../Components.h"

class Entity;

void lua_pushvec2f(lua_State* L, const sf::Vector2f& vec);
void lua_pushvec2i(lua_State* L, const sf::Vector2i& vec);
void lua_pushvec2u(lua_State* L, const sf::Vector2u& vec);
void lua_pushcolor(lua_State* L, const sf::Color& color);
void lua_pushvertex(lua_State* L, const sf::Vertex& vert);

sf::Vector2f lua_tovec2f(lua_State* L, int idx);
sf::Vector2i lua_tovec2i(lua_State* L, int idx);
sf::Vector2u lua_tovec2u(lua_State* L, int idx);
sf::Color lua_tocolor(lua_State* L, int idx);
sf::Vertex lua_tovertex(lua_State* L, int idx);

void lua_pushtransformRef(lua_State* L, sf::Transformable& tf);
void lua_pushvertexarrayRef(lua_State* L, sf::VertexArray& va);
void lua_pushmaterialRef(lua_State* L, MaterialComponent& mat);
void lua_pushanimationRef(lua_State* L, AnimationComponent& anim);
void lua_pushrigidbodyRef(lua_State* L, b2Body* body);
void lua_pushcolliderRef(lua_State* L, b2Fixture* fix);
void lua_pushscriptRef(lua_State* L, lua_State* script);

void lua_pushtransformVal(lua_State* L, const sf::Transformable& tf);
void lua_pushvertexarrayVal(lua_State* L, const sf::VertexArray& va);
void lua_pushmaterialVal(lua_State* L,  MaterialComponent& mat);
void lua_pushanimationVal(lua_State* L, const AnimationComponent::_Animation& anim);