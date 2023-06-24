#include "PCH.h"
#include "Scene.h"

#include "ecs/Entity.h"
#include "ecs/Components.h"

#include "ecs/scripting/LuaInit.h"
#include "ecs/scripting/LuaEntity.h"
#include "ecs/scripting/LuaConvert.h"


static bool lua_isvalid(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::cout << lua_tostring(L, -1);
		return false;
	}
	return true;
}


void Scene::loadAssets(const rapidjson::Value& assets)
{
	for (const auto& tex : assets["textures"].GetArray())
	{
		const char* name = tex["name"].GetString();
		m_textures[name].loadFromFile(tex["file"].GetString());
		m_textures[name].setSmooth(tex["smooth"].IsTrue());
	}

	for (const auto& sha : assets["shaders"].GetArray())
	{
		const char* name = sha["name"].GetString();
		m_shaders[name].loadFromFile(sha["file"].GetString(), sf::Shader::Fragment);

		for (const auto& uni : sha["uniforms"].GetArray())
		{
			const char* uniname = uni["name"].GetString();

			if (strncmp(uni["type"].GetString(), "sampler", 32) == 0)
				m_shaders[name].setUniform(uniname, sf::Shader::CurrentTexture);
			else if (strncmp(uni["type"].GetString(), "float", 32) == 0)
				m_shaders[name].setUniform(uniname, uni["value"].GetFloat());
		}
	}
}

void Scene::loadWorld(const rapidjson::Value& world)
{
	std::vector<size_t> mapdata;
	sf::Vector2u dimensions(0, 0);

	for (const auto& row : world["map"].GetArray())
	{
		dimensions.x = row.Size();
		dimensions.y += 1;

		for (const auto& idx : row.GetArray())
			mapdata.push_back(idx.GetUint());
	}

	m_background.create(dimensions);
	m_background.loadFromIndices(m_textures["tileset"], sf::Vector2u(4, 4), mapdata.data());

	m_camera.reset(sf::FloatRect(0, 0, 480, 270));

	m_physicssim = std::make_unique<b2World>(b2Vec2(0, 0));
	m_physicssim->SetContactListener(&m_collisions);
}

void Scene::loadEntities(const rapidjson::Value& entities)
{
	std::unordered_set<std::string> tags;

	for (const auto& e : entities.GetArray())
	{
		Entity entity = Entity(this);

		const std::string& tag = entity.addComponent<TagComponent>(e["tag"].GetString()).tag;
		auto [_, success] = tags.insert(tag);
		if (!success)
			throw EntityError("Multiple use of identical tag");

		const auto& tfdata = e["transform"];
		sf::Transformable& tf = entity.addComponent<TransformComponent>().transform;
		const sf::Vector2f pos(tfdata["loc"][0].GetFloat(), tfdata["loc"][1].GetFloat());
		const float rot = tfdata["rot"].GetFloat();
		const sf::Vector2f scl(tfdata["scl"][0].GetFloat(), tfdata["scl"][1].GetFloat());
		tf.setPosition(pos);
		tf.setRotation(rot);
		tf.setScale(scl);

		const std::string& _layer = entity.addComponent<LayerComponent>(e["layer"].GetString()).layer;
		

		const auto mtdata = e.FindMember("material");
		if (mtdata != e.MemberEnd())
		{
			auto& mt = entity.addComponent<MaterialComponent>();
			if (!mtdata->value["texture"].IsNull())
				mt.texture = &m_textures[mtdata->value["texture"].GetString()];
			if (!mtdata->value["shader"].IsNull())
				mt.shader = &m_shaders[mtdata->value["shader"].GetString()];
		}

		const auto vadata = e.FindMember("vertex-array");
		if (vadata != e.MemberEnd())
		{
			auto& va = entity.addComponent<VertexArrayComponent>().vertexarray;
			va.setPrimitiveType(static_cast<sf::PrimitiveType>(vadata->value["primitive"].GetInt()));
			const auto& verts = vadata->value["vertices"];
			if (verts.IsNull())
			{
				if (auto mt = entity.getComponentIfHas<MaterialComponent>())
				{
					if (!mt->texture)
						return;

					const sf::Vector2f qsize(mt->texture->getSize());
					const auto& col = vadata->value["col"];
					const sf::Color qcol(col[0].GetUint(), col[1].GetUint(),
						col[2].GetUint(), col[3].GetUint());
					va.append(sf::Vertex(sf::Vector2f(0, 0), qcol, sf::Vector2f(0, 0)));
					va.append(sf::Vertex(sf::Vector2f(qsize.x, 0), qcol, sf::Vector2f(qsize.x, 0)));
					va.append(sf::Vertex(sf::Vector2f(qsize.x, qsize.y), qcol, sf::Vector2f(qsize.x, qsize.y)));
					va.append(sf::Vertex(sf::Vector2f(0, qsize.y), qcol, sf::Vector2f(0, qsize.y)));
				}
			}
			else
			{
				for (const auto& v : vadata->value["vertices"].GetArray())
				{
					const sf::Vector2f vpos(v["pos"][0].GetFloat(), v["pos"][1].GetFloat());
					const sf::Color vcol(v["col"][0].GetUint(), v["col"][1].GetUint(), v["col"][2].GetUint(), v["col"][3].GetUint());
					const sf::Vector2f vtex(v["tex"][0].GetFloat(), v["tex"][1].GetFloat());
					va.append(sf::Vertex(vpos, vcol, vtex));
				}
			}
		}

		const auto andata = e.FindMember("animation");
		if (andata != e.MemberEnd())
		{
			if (vadata == e.MemberEnd())
				return;

			if (auto mt = entity.getComponentIfHas<MaterialComponent>())
			{
				if (!mt->texture)
					return;

				auto& anim = entity.addComponent<AnimationComponent>().animation;
				anim = std::make_unique<AnimationComponent::_Animation>();

				anim->animcount = andata->value["anim-count"].GetUint();
				anim->framecount = andata->value["frame-count"].GetUint();

				const sf::Vector2f texsize(mt->texture->getSize());
				anim->cellsize = sf::Vector2u(static_cast<uint32_t>(texsize.x) / anim->framecount, static_cast<uint32_t>(texsize.y) / anim->animcount);

				anim->anim = andata->value["anim-begin"].GetUint();
				anim->frame = andata->value["frame-begin"].GetUint();

				anim->playing = andata->value["play"].IsTrue();
				anim->loop = andata->value["loop"].IsTrue();

				anim->frametime = andata->value["frame-time"].GetFloat();

				sf::VertexArray& va = entity.getComponent<VertexArrayComponent>().vertexarray;

				va[0].position = sf::Vector2f(0, 0);
				va[1].position = sf::Vector2f(static_cast<float>(anim->cellsize.x), 0);
				va[2].position = sf::Vector2f(anim->cellsize);
				va[3].position = sf::Vector2f(0, static_cast<float>(anim->cellsize.y));

				va[0].texCoords = sf::Vector2f(static_cast<float>(anim->frame * anim->cellsize.x),						static_cast<float>(anim->anim * anim->cellsize.y));
				va[1].texCoords = sf::Vector2f(static_cast<float>(anim->frame * anim->cellsize.x + anim->cellsize.x),	static_cast<float>(anim->anim * anim->cellsize.y));
				va[2].texCoords = sf::Vector2f(static_cast<float>(anim->frame * anim->cellsize.x + anim->cellsize.x),	static_cast<float>(anim->anim * anim->cellsize.y + anim->cellsize.y));
				va[3].texCoords = sf::Vector2f(static_cast<float>(anim->frame * anim->cellsize.x),						static_cast<float>(anim->anim * anim->cellsize.y + anim->cellsize.y));
			}
		}

		const auto rbdata = e.FindMember("rigidbody");
		if (rbdata != e.MemberEnd())
		{
			const auto& rb = rbdata->value;
			const b2Vec2 physpos = b2Vec2(pos.x * toPhys, pos.y * toPhys);

			b2BodyDef bodydef;
			bodydef.type = static_cast<b2BodyType>(rb["type"].GetInt());
			bodydef.position = physpos;
			bodydef.angle = rb["angle"].GetFloat();
			bodydef.fixedRotation = rb["fixed-rotation"].IsTrue();
			bodydef.bullet = rb["bullet"].IsTrue();
			bodydef.enabled = rb["enabled"].IsTrue();
			bodydef.userData.pointer = static_cast<uintptr_t>(entity.getEnttID());

			auto& body = entity.addComponent<RigidBodyComponent>(m_physicssim->CreateBody(&bodydef)).body;

			bool hascollider = false;

			for (const auto& cl : rb["colliders"].GetArray())
			{
				hascollider = true;

				b2FixtureDef fixturedef;
				fixturedef.density = cl["density"].GetFloat();
				fixturedef.friction = cl["friction"].GetFloat();
				fixturedef.isSensor = cl["is-sensor"].IsTrue();

				b2PolygonShape polyshape;
				b2CircleShape circleshape;
				b2ChainShape chainshape;

				const char* shapestr = cl["shape"].GetString();
				if (strncmp(shapestr, "box", 32) == 0)
				{
					const b2Vec2 offset = b2Vec2(cl["offset"][0].GetFloat() * toPhys, cl["offset"][1].GetFloat() * toPhys);
					const b2Vec2 size = b2Vec2(cl["size"][0].GetFloat() * toPhys, cl["size"][1].GetFloat() * toPhys);
					const b2Vec2 quad[4] = { offset + b2Vec2(0, 0), offset + b2Vec2(size.x, 0), offset + size, offset + b2Vec2(0, size.x) };
					polyshape.Set(quad, 4);

					fixturedef.shape = &polyshape;
				}
				else if (strncmp(shapestr, "circle", 32) == 0)
				{
					const float radius = cl["radius"].GetFloat() * toPhys;
					circleshape.m_radius = radius;
					circleshape.m_p.Set(cl["offset"][0].GetFloat() * toPhys, cl["offset"][1].GetFloat()* toPhys);
					fixturedef.shape = &circleshape;
				}
				else if (strncmp(shapestr, "chain", 32) == 0)
				{
					std::vector<b2Vec2> verts;
					for (const auto& v : cl["vertices"].GetArray())
						verts.emplace_back(v[0].GetFloat() * toPhys, v[1].GetFloat() * toPhys);

					if (cl["loop"].IsTrue())
						chainshape.CreateLoop(verts.data(), static_cast<int32>(verts.size()));
					else
						chainshape.CreateChain(&verts[1], static_cast<int32>(verts.size() - 2), verts.front(), verts.back());

					fixturedef.shape = &chainshape;
				}

				const auto iddata = cl.FindMember("id");
				if (iddata != cl.MemberEnd())
					fixturedef.userData.pointer = (uintptr_t)_strdup(iddata->value.GetString());

				body->CreateFixture(&fixturedef);
			}

			if (hascollider)
				entity.addComponent<ColliderComponent>().fixture = body->GetFixtureList();
		}

		const auto mvdata = e.FindMember("movement");
		if (mvdata != e.MemberEnd())
		{
			auto& mv = entity.addComponent<MovementComponent>();

			mv.type = static_cast<MovementComponent::Type>(mvdata->value["type"].GetInt());
			mv.speed = mvdata->value["speed"].GetFloat();
			mv.locked = mvdata->value["locked"].IsTrue();
		}

		const auto stdata = e.FindMember("script");
		if (stdata != e.MemberEnd())
		{
			auto& sc = entity.addComponent<ScriptComponent>();

			sc.L = luaL_newstate();
			luaL_openlibs(sc.L);

			const char* file = stdata->value.GetString();
			if (lua_isvalid(sc.L, luaL_dofile(sc.L, file)))
			{
				lua_pushlightuserdata(sc.L, this);
				lua_setglobal(sc.L, "Scene");

				lua_pushentity(sc.L, entity);
				lua_setglobal(sc.L, "Self");

				lua_registerEngineFuncs(sc.L);
				lua_setOverridenFuncs(sc.L, sc.funcs);
				lua_setCoroutines(sc.L, sc.coroutines);
			}
		}
	}
}


void Scene::callbacks(const sf::Event& event, ScriptComponent& sc)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (sc.funcs & ScriptComponent::OnMouseClick)
		{
			lua_getglobal(sc.L, "OnMouseClick");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			lua_pushnumber(sc.L, event.mouseButton.button);
			lua_call(sc.L, 2, 0);
		}
		break;
	case sf::Event::MouseMoved:
		if (sc.funcs & ScriptComponent::OnMouseMove)
		{
			lua_getglobal(sc.L, "OnMouseMove");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			lua_call(sc.L, 1, 0);
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (sc.funcs & ScriptComponent::OnMouseRelease)
		{
			lua_getglobal(sc.L, "OnMouseRelease");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			lua_pushnumber(sc.L, event.mouseButton.button);
			lua_call(sc.L, 2, 0);
		}
		break;
	case sf::Event::MouseWheelScrolled:
		if (sc.funcs & ScriptComponent::OnMouseScroll)
		{
			lua_getglobal(sc.L, "OnMouseScroll");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
			lua_pushnumber(sc.L, event.mouseWheelScroll.delta);
			lua_call(sc.L, 2, 0);
		}
		break;
	case sf::Event::KeyPressed:
		if (sc.funcs & ScriptComponent::OnKeyPress)
		{
			lua_getglobal(sc.L, "OnKeyPress");
			lua_pushnumber(sc.L, event.key.code);
			lua_call(sc.L, 1, 0);
		}
		break;
	case sf::Event::KeyReleased:
		if (sc.funcs & ScriptComponent::OnKeyRelease)
		{
			lua_getglobal(sc.L, "OnKeyRelease");
			lua_pushnumber(sc.L, event.key.code);
			lua_call(sc.L, 1, 0);
		}
		break;
	}
}

void Scene::asyncCallbacks(const sf::Event& event, ScriptComponent& sc)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed: {
		auto& asfunc = sc.coroutines[ScriptComponent::OnMouseClick];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc.L, "OnMouseClickAsync");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			lua_pushnumber(sc.L, event.mouseButton.button);
			int retvalcount = 1;
			if (lua_resume(sc.L, asfunc.thread, 2, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)//!lua_isnil(asfunc.thread, -1))
				asfunc.delay = static_cast<float>(lua_tonumber(sc.L, -1));
		}
		break; }
	case sf::Event::MouseMoved: {
		auto& asfunc = sc.coroutines[ScriptComponent::OnMouseMove];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc.L, "OnMouseMoveAsync");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			int retvalcount = 1;
			if (lua_resume(sc.L, asfunc.thread, 2, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)//!lua_isnil(asfunc.thread, -1))
				asfunc.delay = static_cast<float>(lua_tonumber(sc.L, -1));
		}
		break; }
	case sf::Event::MouseButtonReleased:
		if (sc.funcs & ScriptComponent::OnMouseRelease)
		{
			lua_getglobal(sc.L, "OnMouseRelease");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			lua_pushnumber(sc.L, event.mouseButton.button);
			lua_call(sc.L, 2, 0);
		}
		break;
	case sf::Event::MouseWheelScrolled:
		if (sc.funcs & ScriptComponent::OnMouseScroll)
		{
			lua_getglobal(sc.L, "OnMouseScroll");
			lua_pushvec2i(sc.L, sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
			lua_pushnumber(sc.L, event.mouseWheelScroll.delta);
			lua_call(sc.L, 2, 0);
		}
		break;
	case sf::Event::KeyPressed:
		if (sc.funcs & ScriptComponent::OnKeyPress)
		{
			lua_getglobal(sc.L, "OnKeyPress");
			lua_pushnumber(sc.L, event.key.code);
			lua_call(sc.L, 1, 0);
		}
		break;
	case sf::Event::KeyReleased:
		if (sc.funcs & ScriptComponent::OnKeyRelease)
		{
			lua_getglobal(sc.L, "OnKeyRelease");
			lua_pushnumber(sc.L, event.key.code);
			lua_call(sc.L, 1, 0);
		}
		break;
	}
}


Scene::Scene(const std::string& filename)
	: m_collisions(this)
{
	loadFromFile(filename);

	auto scripts = m_registry.view<ScriptComponent>();
	for (const entt::entity e : scripts)
	{
		auto& sc = scripts.get<ScriptComponent>(e);

		if (sc.funcs & ScriptComponent::OnCreate)
		{
			lua_getglobal(sc.L, "OnCreate");
			lua_call(sc.L, 0, 0);
		}
	}
}


void Scene::loadFromFile(const std::string& filename)
{
	std::ifstream stream(filename);

	stream.seekg(0, std::ios::end);
	const size_t size = static_cast<size_t>(stream.tellg());
	stream.seekg(0, std::ios::beg);

	auto buffer = std::make_unique<char[]>(size + 1);
	stream.read(buffer.get(), size);
	buffer[size] = '\0';

	rapidjson::Document doc;
	doc.Parse(buffer.get());


	loadAssets(doc["assets"]);
	loadWorld(doc["world"]);
	loadEntities(doc["entities"]);
}


sf::Vector2f Scene::screenToWorld(const sf::Vector2i& vec) const
{
	return p_window->mapPixelToCoords(vec);
}

sf::Vector2i Scene::worldToScreen(const sf::Vector2f& vec) const
{
	return p_window->mapCoordsToPixel(vec);
}

sf::Vector2f Scene::screenToPhys(const sf::Vector2i& vec) const
{
	return toPhys* screenToWorld(vec);
}

sf::Vector2i Scene::physToScreen(const sf::Vector2f& vec) const
{
	return worldToScreen(toWorld * vec);
}


Entity Scene::findEntityByTag(const char* tag)
{
	auto tags = m_registry.view<TagComponent>();

	for (const entt::entity e : tags)
	{
		if (tags.get<TagComponent>(e).tag == tag)
			return Entity(this, e);
	}

	return Entity(this, entt::null);
}


void Scene::handleEvent(const sf::Event& event)
{
	auto scripts = m_registry.view<ScriptComponent>();

	for (const entt::entity e : scripts)
	{
		auto& sc = scripts.get<ScriptComponent>(e);

		callbacks(event, sc);
		asyncCallbacks(event, sc);
	}
}

void Scene::update(float deltatime)
{
	m_physicssim->Step(deltatime, 6, 4);

	auto bodies = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
	for (const entt::entity e : bodies)
	{
		auto [rb, tf] = bodies.get<RigidBodyComponent, TransformComponent>(e);

		const b2Vec2 physpos = rb.body->GetPosition();
		const sf::Vector2f worldpos(physpos.x * toWorld, physpos.y * toWorld);
		tf.transform.setPosition(worldpos);
	}

	auto movers = m_registry.group<MovementComponent>(entt::get<RigidBodyComponent>);
	for (const entt::entity e : movers)
	{
		auto [mv, rb] = movers.get<MovementComponent, RigidBodyComponent>(e);

		if (!mv.locked)
		{
			switch (mv.type)
			{
			case MovementComponent::Type::Manual: {
				b2Vec2 dir(
					static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					- static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::A)),
					static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					- static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				);

				if (dir.x != 0 && dir.y != 0)
					dir *= oneOverRoot2;
				dir *= mv.speed;

				//rb.body->SetLinearVelocity(dir);
				break; }

			case MovementComponent::Type::Auto:
				break;
			}
		}
	}

	auto anims = m_registry.group<AnimationComponent>(entt::get<VertexArrayComponent>);
	for (const entt::entity e : anims)
	{
		auto [_an, va] = anims.get<AnimationComponent, VertexArrayComponent>(e);
		auto& an = *_an.animation.get();

		if (an.playing)
		{
			if (an.currenttime >= an.frametime)
			{
				an.currenttime -= an.frametime;
				if (++an.frame == an.framecount)
				{
					if (an.loop)
						an.frame = 0;
					else
					{
						an.frame--;
						an.playing = false;
					}
				}

				va.vertexarray[0].texCoords = sf::Vector2f(static_cast<float>(an.frame * an.cellsize.x),					static_cast<float>(an.anim * an.cellsize.y));
				va.vertexarray[1].texCoords = sf::Vector2f(static_cast<float>(an.frame * an.cellsize.x + an.cellsize.x),	static_cast<float>(an.anim * an.cellsize.y));
				va.vertexarray[2].texCoords = sf::Vector2f(static_cast<float>(an.frame * an.cellsize.x + an.cellsize.x),	static_cast<float>(an.anim * an.cellsize.y + an.cellsize.y));
				va.vertexarray[3].texCoords = sf::Vector2f(static_cast<float>(an.frame * an.cellsize.x),					static_cast<float>(an.anim * an.cellsize.y + an.cellsize.y));
			}
			else
				an.currenttime += deltatime;
		}
	}

	auto dead = m_registry.view<DestroyComponent>();
	for (const entt::entity e : dead)
	{
		Entity entity(this, e);

		if (entity.hasComponent<RigidBodyComponent>())
		{
			b2Body* body = entity.getComponent<RigidBodyComponent>().body;

			for (b2Fixture* fix = body->GetFixtureList(); fix; fix = fix->GetNext())
			{
				if (void* name = (void*)fix->GetUserData().pointer)
					free(name);
			}

			m_physicssim->DestroyBody(body);
		}

		m_registry.destroy(e);
	}

	auto scripts = m_registry.view<ScriptComponent>();
	for (const entt::entity e : scripts)
	{
		auto& sc = scripts.get<ScriptComponent>(e);

		if (sc.funcs & ScriptComponent::OnUpdate)
		{
			lua_getglobal(sc.L, "OnUpdate");
			lua_pushnumber(sc.L, deltatime);
			lua_call(sc.L, 1, 0);
		}
		auto& asfunc = sc.coroutines[ScriptComponent::OnUpdate];
		if (asfunc.thread && asfunc.delay <= 0)
		{
			lua_getglobal(sc.L, "OnUpdateAsync");
			lua_pushnumber(sc.L, deltatime);
			int retvalcount = 1;
			if (lua_resume(sc.L, asfunc.thread, 1, &retvalcount) == LUA_YIELD)
				asfunc.resumable = true;
			else
				asfunc.resumable = false;
			if (retvalcount > 0)
				asfunc.delay = static_cast<float>(lua_tonumber(sc.L, -1));
		}
		for (auto& [_, co] : sc.coroutines)
		{
			co.delay -= deltatime;
			if (co.delay <= 0 && co.resumable)
			{
				int retvalcount = 1;
				if (lua_resume(sc.L, co.thread, 0, &retvalcount) == LUA_YIELD)
					co.resumable = true;
				else
					co.resumable = false;
				if (retvalcount > 0)
					co.delay = static_cast<float>(lua_tonumber(sc.L, -1));
			}
		}
	}
}

void Scene::render(sf::RenderTarget& target)
{
	target.setView(m_camera);

	target.draw(m_background);

	const auto renderables = m_registry.group<TransformComponent, VertexArrayComponent, MaterialComponent>();
	for (const entt::entity e : renderables)
	{
		const auto [tf, va, mt] = renderables.get<TransformComponent, VertexArrayComponent, MaterialComponent>(e);

		sf::RenderStates states;
		states.texture = mt.texture;
		states.transform = tf.transform.getTransform();
		states.shader = mt.shader;
		target.draw(va.vertexarray, states);
	}
}