#include "Paddle.hpp"
#include <algorithm>

constexpr unsigned VIRTUAL_HEIGHT = 243;

Paddle::Paddle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{
}

void Paddle::update(float dt)
{
	y += dy * dt;
	y = std::clamp(y, 0.f, (float)VIRTUAL_HEIGHT - height);
}

void Paddle::render(sf::RenderWindow &window) const
{
	sf::RectangleShape shape({width, height});
	shape.setPosition({x, y});
	window.draw(shape);
}