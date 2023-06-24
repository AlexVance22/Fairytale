#pragma once

#include "physics/Collision.h"
#include "physics/Utility.h"
#include "graphics/TileMap.h"


struct ScriptComponent;
class Entity;


class Scene
{
private:
	entt::registry m_registry;

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::Shader> m_shaders;

	TileMap m_background;
	sf::View m_camera;

	std::unique_ptr<b2World> m_physicssim;
	ContactListener m_collisions;

private:
	void loadAssets(const rapidjson::Value& assets);
	void loadWorld(const rapidjson::Value& world);
	void loadEntities(const rapidjson::Value& entities);

	void callbacks(const sf::Event& event, ScriptComponent& sc);
	void asyncCallbacks(const sf::Event& event, ScriptComponent& sc);

public:
	sf::RenderWindow* p_window = nullptr;

public:
	Scene(const std::string& filename);

	void loadFromFile(const std::string& filename);

	sf::Vector2f screenToWorld(const sf::Vector2i& vec) const;
	sf::Vector2i worldToScreen(const sf::Vector2f& vec) const;
	sf::Vector2f screenToPhys(const sf::Vector2i& vec) const;
	sf::Vector2i physToScreen(const sf::Vector2f& vec) const;

	Entity findEntityByTag(const char* tag);

	void handleEvent(const sf::Event& event);
	void update(float deltatime);
	void render(sf::RenderTarget& target);


	friend class Entity;
};