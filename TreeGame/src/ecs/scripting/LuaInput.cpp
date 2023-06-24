#include "PCH.h"
#include "LuaInput.h"


int l_queryKey(lua_State* L)
{
	const auto key = static_cast<sf::Keyboard::Key>(lua_tonumber(L, 1));

	const int state = sf::Keyboard::isKeyPressed(key);

	lua_pushnumber(L, state);

	return 1;
}

int l_queryMouse(lua_State* L)
{
	const auto button = static_cast<sf::Mouse::Button>(lua_tonumber(L, 1));

	const int state = sf::Mouse::isButtonPressed(button);

	lua_pushnumber(L, state);

	return 1;
}