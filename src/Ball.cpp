#include "Ball.hpp"
#include "Constants.hpp"
#include "Random.hpp"

Ball::Ball(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{
}

void Ball::reset(int servingPlayer)
{
	x = Constants::VIRTUAL_WIDTH / 2.f - width / 2.f;
	y = Constants::VIRTUAL_HEIGHT / 2.f - height / 2.f;
	std::uniform_real_distribution<float> randDY(-50.f, 50.f);
	dx = (servingPlayer == 1) ? 100.f : -100.f;
	dy = randDY(Random::rng());
}

void Ball::serve()
{
	std::uniform_real_distribution<float> randDY(-50.f, 50.f);
	std::bernoulli_distribution coin;
	dx = coin(Random::rng()) ? 100.f : -100.f;
	dy = randDY(Random::rng());
}

void Ball::update(float dt)
{
	x += dx * dt;
	y += dy * dt;
}

sf::FloatRect Ball::bounds() const
{
	return sf::FloatRect({x, y}, {width, height});
}

void Ball::render(sf::RenderWindow &window) const
{
	sf::RectangleShape shape({width, height});
	shape.setPosition({x, y});
	window.draw(shape);
}