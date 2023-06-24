#pragma once


struct TagComponent
{
	std::string tag;
};

struct TransformComponent
{
	sf::Transformable transform;
};

struct LayerComponent
{
	std::string layer;
};

struct VertexArrayComponent
{
	sf::VertexArray vertexarray;
};

struct MaterialComponent
{
	sf::Texture* texture = nullptr;
	sf::Shader* shader = nullptr;
};

struct AnimationComponent
{
	struct _Animation
	{
		uint32_t animcount = 0;
		uint32_t framecount = 0;
		sf::Vector2u cellsize;

		uint32_t anim = 0;
		uint32_t frame = 0;
		float frametime = 0;
		float currenttime = 0;

		bool playing = false;
		bool loop = false;
	};

	std::unique_ptr<_Animation> animation = nullptr;
};

struct MovementComponent
{
	enum class Type { Manual, Auto };

	Type type = Type::Auto;
	float speed = 0;
	bool locked = false;
};

struct DestroyComponent
{
	char dummy = 0;
};


struct ColliderComponent
{
	b2Fixture* fixture = nullptr;
};

struct RigidBodyComponent
{
	b2Body* body = nullptr;
};


struct ScriptComponent
{
	enum Func
	{
		None			= 0,

		OnHit			= 1,
		OnExit			= 2,

		OnMouseClick	= 2 << 1,
		OnMouseMove		= 2 << 2,
		OnMouseRelease	= 2 << 3,
		OnMouseScroll	= 2 << 4,

		OnKeyPress		= 2 << 5,
		OnKeyRelease	= 2 << 6,

		OnCreate		= 2 << 7,
		OnUpdate		= 2 << 8,
		OnDestroy		= 2 << 9,
	};


	lua_State* L = nullptr;
	uint32_t funcs = Func::None;

	struct AsyncFunc
	{
		lua_State* thread = nullptr;
		bool resumable = false;
		float delay = 0;
	};

	std::unordered_map<Func, AsyncFunc> coroutines;
};