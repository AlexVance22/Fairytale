#pragma once

class Scene;


class ContactListener : public b2ContactListener
{
private:
	Scene* p_scene = nullptr;

private:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

public:
	ContactListener(Scene* scene);
};