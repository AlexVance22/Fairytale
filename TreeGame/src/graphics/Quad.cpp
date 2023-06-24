#include "PCH.h"
#include "Quad.h"


void q_setBounds(sf::Vertex* quad, const sf::FloatRect& rect)
{
	q_setPosition(quad, sf::Vector2f(rect.left, rect.top));
	q_setSize(quad, sf::Vector2f(rect.width, rect.height));
}

void q_setPosition(sf::Vertex* quad, const sf::Vector2f& position)
{
	const sf::Vector2f size = q_getSize(quad);

	quad[0].position = position;
	quad[1].position = sf::Vector2f(position.x + size.x, position.y);
	quad[2].position = position + size;
	quad[3].position = sf::Vector2f(position.x, position.y + size.y);
}

void q_setSize(sf::Vertex* quad, const sf::Vector2f& size)
{
	const sf::Vector2f position = q_getPosition(quad);

	quad[1].position = sf::Vector2f(position.x + size.x, position.y);
	quad[2].position = position + size;
	quad[3].position = sf::Vector2f(position.x, position.y + size.y);
}

void q_move(sf::Vertex* quad, const sf::Vector2f& move)
{
	q_setPosition(quad, q_getPosition(quad) + move);
}

void q_setTextureRect(sf::Vertex* quad, const sf::FloatRect& rect)
{
	const sf::Vector2f position = sf::Vector2f(rect.left, rect.top);
	const sf::Vector2f size = sf::Vector2f(rect.width, rect.height);

	quad[0].texCoords = position;
	quad[1].texCoords = sf::Vector2f(position.x + size.x, position.y);
	quad[2].texCoords = position + size;
	quad[3].texCoords = sf::Vector2f(position.x, position.y + size.y);
}

void q_setColor(sf::Vertex* quad, const sf::Color& color)
{
	for (size_t i = 0; i < 4; i++)
		quad[i].color = color;
}


sf::FloatRect q_getBounds(const sf::Vertex* quad)
{
	return sf::FloatRect(q_getPosition(quad), q_getSize(quad));
}

sf::Vector2f q_getPosition(const sf::Vertex* quad)
{
	return quad[0].position;
}

sf::Vector2f q_getSize(const sf::Vertex* quad)
{
	return quad[2].position - quad[0].position;
}

sf::FloatRect q_getTextureRect(const sf::Vertex* quad)
{
	const sf::Vector2f position = quad[0].texCoords;
	const sf::Vector2f size = quad[2].texCoords - quad[0].texCoords;

	return sf::FloatRect(position, size);
}

sf::Color q_getColor(sf::Vertex* quad)
{
	return quad[0].color;
}