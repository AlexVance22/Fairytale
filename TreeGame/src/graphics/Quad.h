#pragma once


void q_setBounds(sf::Vertex* quad, const sf::FloatRect& rect);
void q_setPosition(sf::Vertex* quad, const sf::Vector2f& position);
void q_setSize(sf::Vertex* quad, const sf::Vector2f& size);
void q_move(sf::Vertex* quad, const sf::Vector2f& move);
void q_setTextureRect(sf::Vertex* quad, const sf::FloatRect& rect);
void q_setColor(sf::Vertex* quad, const sf::Color& color);

sf::FloatRect q_getBounds(const sf::Vertex* quad);
sf::Vector2f q_getPosition(const sf::Vertex* quad);
sf::Vector2f q_getSize(const sf::Vertex* quad);
sf::FloatRect q_getTextureRect(const sf::Vertex* quad);
sf::Color q_getColor(sf::Vertex* quad);