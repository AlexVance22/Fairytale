#pragma once


extern const float toPhys;
extern const float toWorld;
extern const float oneOverRoot2;


sf::Vector2f b2VecToSfVec(const b2Vec2& vec);
b2Vec2 sfVecToB2Vec(const sf::Vector2f& vec);