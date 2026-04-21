#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Ball
{
  public:
	float x, y;
	float width, height;
	float dx = 0.f, dy = 0.f;

	Ball(float x, float y, float width, float height);

	void reset();
	void update(float dt);
	void render(sf::RenderWindow &window) const;

  private:
	static std::mt19937 &rng();
};