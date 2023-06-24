#include "PCH.h"
#include "TileMap.h"

#include "Quad.h"


void TileMap::setTexture(const sf::Texture& texture, const sf::Vector2u& texdims)
{
	m_texture = &texture;
	m_texdims = texdims;
	const sf::Vector2u texsize = texture.getSize();
	m_cellsize = sf::Vector2u(texsize.x / m_texdims.x, texsize.y / m_texdims.y);
}


TileMap::TileMap(sf::Vector2u dimensions)
{
	create(dimensions);
}

void TileMap::create(sf::Vector2u dimensions)
{
	m_dims = dimensions;
	m_quads.resize(m_dims.x * m_dims.y);

	for (size_t i = 0; i < m_dims.x * m_dims.y; i++)
		m_quads.emplaceBack(sf::FloatRect(0, 0, 0, 0));
}


void TileMap::loadFromIndices(const sf::Texture & texture, const sf::Vector2u & texdims, const size_t * indices)
{
	setTexture(texture, texdims);

	for (size_t i = 0; i < m_dims.x * m_dims.y; i++)
	{
		const sf::Vector2f position(
			static_cast<float>(i % m_dims.x) * m_cellsize.x,
			static_cast<float>(i / m_dims.x) * m_cellsize.y
		);

		q_setBounds(m_quads[i], sf::FloatRect(position, static_cast<sf::Vector2f>(m_cellsize)));

		const sf::Vector2f texpos(
			static_cast<float>(indices[i] % m_texdims.x) * m_cellsize.x,
			static_cast<float>(indices[i] / m_texdims.x) * m_cellsize.y
		);

		q_setTextureRect(m_quads[i], sf::FloatRect(texpos, static_cast<sf::Vector2f>(m_cellsize)));
	}
}

void TileMap::loadFromUVs(const sf::Texture & texture, const sf::Vector2u & texdims, const sf::Vector2u * uvs)
{
	setTexture(texture, texdims);

	for (size_t i = 0; i < m_dims.x * m_dims.y; i++)
	{
		const sf::Vector2f position(
			static_cast<float>(i % m_dims.x) * m_cellsize.x,
			static_cast<float>(i / m_dims.x) * m_cellsize.y
		);

		q_setBounds(m_quads[i], sf::FloatRect(position, static_cast<sf::Vector2f>(m_cellsize)));

		q_setTextureRect(m_quads[i], sf::FloatRect(static_cast<sf::Vector2f>(uvs[i]), static_cast<sf::Vector2f>(m_cellsize)));
	}
}


void TileMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_texture;

	target.draw(m_quads, states);
}