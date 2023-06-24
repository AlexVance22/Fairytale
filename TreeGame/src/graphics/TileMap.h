#pragma once

#include "QuadArray.h"


class TileMap : public sf::Drawable, public sf::Transformable
{
private:
	const sf::Texture* m_texture = nullptr;
	QuadArray m_quads;

	sf::Vector2u m_dims;
	sf::Vector2u m_texdims;
	sf::Vector2u m_cellsize;

private:
	void setTexture(const sf::Texture& texture, const sf::Vector2u& texdims);

public:
	TileMap() = default;
	TileMap(sf::Vector2u dimensions);

	void create(sf::Vector2u dimensions);

	void loadFromIndices(const sf::Texture& texture, const sf::Vector2u& texdims, const size_t* indices);
	void loadFromUVs(const sf::Texture& texture, const sf::Vector2u& texdims, const sf::Vector2u* uvs);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};