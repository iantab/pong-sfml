#include "Constants.hpp"
#include "Paddle.hpp"
#include <algorithm>

Paddle::Paddle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{
}

void Paddle::update(float dt)
{
	y += dy * dt;
	y = std::clamp(y, 0.f, Constants::VIRTUAL_HEIGHT - height);
}

sf::FloatRect Paddle::bounds() const
{
	return sf::FloatRect({x, y}, {width, height});
}

void Paddle::render(sf::RenderWindow &window) const
{
	sf::RectangleShape shape({width, height});
	shape.setPosition({x, y});
	window.draw(shape);
}