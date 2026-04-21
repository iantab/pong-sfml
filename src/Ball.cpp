#include "Ball.hpp"

constexpr unsigned VIRTUAL_WIDTH = 432;
constexpr unsigned VIRTUAL_HEIGHT = 243;

std::mt19937 &Ball::rng()
{
	static std::mt19937 engine{std::random_device{}()};
	return engine;
}

Ball::Ball(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{
}

void Ball::reset()
{
	x = VIRTUAL_WIDTH / 2.f - width / 2.f;
	x = VIRTUAL_WIDTH / 2.f - width / 2.f;
	y = VIRTUAL_HEIGHT / 2.f - height / 2.f;
	std::uniform_real_distribution<float> randDY(-50.f, 50.f);
	dx = (rng()() % 2 == 0) ? 100.f : -100.f;
	dy = randDY(rng());
}

void Ball::update(float dt)
{
	x += dx * dt;
	y += dy * dt;
}

void Ball::render(sf::RenderWindow &window) const
{
	sf::RectangleShape shape({width, height});
	shape.setPosition({x, y});
	window.draw(shape);
}