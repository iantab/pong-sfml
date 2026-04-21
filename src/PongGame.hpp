#pragma once
#include "Ball.hpp"
#include "Constants.hpp"
#include "Paddle.hpp"
#include <SFML/Graphics.hpp>

class PongGame
{
  public:
	PongGame();
	void run();

  private:
	enum class State
	{
		Start,
		Play
	};

	void handleEvents();
	void update(float dt);
	void draw();

	sf::RenderWindow window;
	sf::Font font;
	sf::Text scoreLeft, scoreRight, fpsText;

	Paddle player1, player2;

	Ball ball;

	State state = State::Start;

	sf::Clock fpsClock;
	int fpsFrames = 0;
	int fpsValue = 0;
};