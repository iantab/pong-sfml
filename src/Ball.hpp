#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
  public:
	float x, y;
	float width, height;
	float dx = 0.f, dy = 0.f;

	Ball(float x, float y, float width, float height);

	void reset(int servingPlayer);
	void serve();
	void update(float dt);
	sf::FloatRect bounds() const;
	void render(sf::RenderWindow &window) const;
};