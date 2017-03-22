template <typename T>
T length(const sf::Vector2<T>& vector)
{
	return static_cast<T>(std::sqrt(vector.x * vector.x + vector.y * vector.y));
}

template <typename T>
T lengthSquared(const sf::Vector2<T>& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}