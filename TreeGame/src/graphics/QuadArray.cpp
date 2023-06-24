#include "PCH.h"
#include "QuadArray.h"

#include "Quad.h"


QuadArray::Iterator::Iterator(sf::Vertex* ptr)
	: m_ptr(ptr)
{}

QuadArray::Iterator QuadArray::Iterator::operator++()
{
	m_ptr += 4;
	return Iterator(m_ptr);
}
QuadArray::Iterator QuadArray::Iterator::operator++(int)
{
	sf::Vertex* temp = m_ptr;
	m_ptr += 4;
	return Iterator(temp);
}
QuadArray::Iterator QuadArray::Iterator::operator--()
{
	m_ptr -= 4;
	return Iterator(m_ptr);
}
QuadArray::Iterator QuadArray::Iterator::operator--(int)
{
	sf::Vertex* temp = m_ptr;
	m_ptr -= 4;
	return Iterator(temp);
}

bool QuadArray::Iterator::operator==(const Iterator& other) const
{
	return m_ptr == other.m_ptr;
}
bool QuadArray::Iterator::operator!=(const Iterator& other) const
{
	return m_ptr != other.m_ptr;
}

sf::Vertex* QuadArray::Iterator::operator*()
{
	return m_ptr;
}
const sf::Vertex* QuadArray::Iterator::operator*() const
{
	return m_ptr;
}


QuadArray::ConstIterator::ConstIterator(const sf::Vertex* ptr)
	: m_ptr(ptr)
{}

QuadArray::ConstIterator QuadArray::ConstIterator::operator++()
{
	m_ptr += 4;
	return ConstIterator(m_ptr);
}
QuadArray::ConstIterator QuadArray::ConstIterator::operator++(int)
{
	const sf::Vertex* temp = m_ptr;
	m_ptr += 4;
	return ConstIterator(temp);
}
QuadArray::ConstIterator QuadArray::ConstIterator::operator--()
{
	m_ptr -= 4;
	return ConstIterator(m_ptr);
}
QuadArray::ConstIterator QuadArray::ConstIterator::operator--(int)
{
	const sf::Vertex* temp = m_ptr;
	m_ptr -= 4;
	return ConstIterator(temp);
}

bool QuadArray::ConstIterator::operator==(const ConstIterator& other) const
{
	return m_ptr == other.m_ptr;
}
bool QuadArray::ConstIterator::operator!=(const ConstIterator& other) const
{
	return m_ptr != other.m_ptr;
}

const sf::Vertex* QuadArray::ConstIterator::operator*() const
{
	return m_ptr;
}


QuadArray::QuadArray(size_t size)
{
	m_vertices.resize(size);
}

size_t QuadArray::size() const
{
	return m_vertices.size() / 4;
}

sf::Vertex* QuadArray::operator[](size_t index)
{
	return &m_vertices[index * 4];
}
const sf::Vertex* QuadArray::operator[](size_t index) const
{
	return &m_vertices[index * 4];
}

QuadArray::Iterator QuadArray::begin()
{
	return Iterator(&m_vertices[0]);
}
QuadArray::Iterator QuadArray::end()
{
	return Iterator(operator[](size()));
}
QuadArray::ConstIterator QuadArray::begin() const
{
	return ConstIterator(&m_vertices[0]);
}
QuadArray::ConstIterator QuadArray::end() const
{
	return ConstIterator(operator[](size()));
}

sf::Vertex* QuadArray::emplaceBack(const sf::FloatRect& quad)
{
	size_t last = size();

	for (size_t i = 0; i < 4; i++)
		m_vertices.emplace_back();

	q_setBounds(operator[](last), quad);

	return operator[](last);
}
sf::Vertex* QuadArray::insert(const sf::FloatRect& quad, size_t index)
{
	for (size_t i = 0; i < 4; i++)
		m_vertices.emplace(m_vertices.begin() + 4 * index + i);

	q_setBounds(operator[](index), quad);

	return operator[](index);
}
void QuadArray::popBack()
{
	for (size_t i = 4; i < 4; i++)
		m_vertices.pop_back();
}
void QuadArray::erase(size_t index)
{
	for (size_t i = 4; i < 4; i++)
		m_vertices.erase(m_vertices.begin() + index * 4);
}
void QuadArray::resize(size_t count)
{
	m_vertices.resize(count * 4);
}

void QuadArray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);
}