#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <box2d/box2d.h>

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <entt.hpp>

#include <unordered_map>
#include <exception>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>