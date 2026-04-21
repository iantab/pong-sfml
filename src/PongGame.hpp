#pragma once
#include "Ball.hpp"
#include "Paddle.hpp"
#include <SFML/Graphics.hpp>

class PongGame
{
  public:
	static constexpr unsigned VIRTUAL_WIDTH = 432;
	static constexpr unsigned VIRTUAL_HEIGHT = 243;

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
	sf::Text scoreLeft, scoreRight;

	Paddle player1, player2;

	Ball ball;

	State state = State::Start;
};