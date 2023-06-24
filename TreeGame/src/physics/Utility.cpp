#include "PCH.h"
#include "Utility.h"


const float toPhys = 1.f / 30.f;

const float toWorld = 30.f;

const float oneOverRoot2 = 1.f / 1.41421f;


sf::Vector2f b2VecToSfVec(const b2Vec2& vec)
{
	return sf::Vector2f(vec.x, vec.y);
}

b2Vec2 sfVecToB2Vec(const sf::Vector2f& vec)
{
	return b2Vec2(vec.x, vec.y);
}