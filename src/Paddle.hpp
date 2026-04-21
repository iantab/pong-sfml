#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
  public:
	float x, y;
	float width, height;
	float dy = 0.f;

	Paddle(float x, float y, float width, float height);

	void update(float dt);
	sf::FloatRect bounds() const;
	void render(sf::RenderWindow &window) const;
};