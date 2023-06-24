#pragma once


class QuadArray : public sf::Drawable
{
private:
	std::vector<sf::Vertex> m_vertices;

public:
	class Iterator
	{
	private:
		sf::Vertex* m_ptr;

	public:
		Iterator(sf::Vertex* ptr);

		Iterator operator++();
		Iterator operator++(int);
		Iterator operator--();
		Iterator operator--(int);

		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;

		sf::Vertex* operator*();
		const sf::Vertex* operator*() const;
	};
	class ConstIterator
	{
	private:
		const sf::Vertex* m_ptr;

	public:
		ConstIterator(const sf::Vertex* ptr);

		ConstIterator operator++();
		ConstIterator operator++(int);
		ConstIterator operator--();
		ConstIterator operator--(int);

		bool operator==(const ConstIterator& other) const;
		bool operator!=(const ConstIterator& other) const;

		const sf::Vertex* operator*() const;
	};

public:
	QuadArray() = default;
	QuadArray(size_t size);

	size_t size() const;

	sf::Vertex* operator[](size_t index);
	const sf::Vertex* operator[](size_t index) const;

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	sf::Vertex* emplaceBack(const sf::FloatRect& quad);
	sf::Vertex* insert(const sf::FloatRect& quad, size_t index);
	void popBack();
	void erase(size_t index);
	void resize(size_t count);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};