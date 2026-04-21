#include "PongGame.hpp"
#include <optional>

constexpr float PADDLE_SPEED = 200.f;
const sf::Color BG(40, 45, 52);

PongGame::PongGame()
	: window(sf::VideoMode({1280u, 720u}), "Pong"), font("assets/font.ttf"), scoreLeft(font, "0", 32),
	  scoreRight(font, "0", 32), player1(10.f, VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f),
	  player2(VIRTUAL_WIDTH - 15.f, VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f),
	  ball(VIRTUAL_WIDTH / 2.f - 2, VIRTUAL_HEIGHT / 2.f - 2, 4.f, 4.f)
{
	window.setVerticalSyncEnabled(true);
	sf::View view(sf::FloatRect({0.f, 0.f}, {(float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT}));
	window.setView(view);
	font.setSmooth(false);
	scoreLeft.setPosition({VIRTUAL_WIDTH / 2.f - 50, 30.f});
	scoreRight.setPosition({VIRTUAL_WIDTH / 2.f + 30, 30.f});
}

void PongGame::run()
{
	sf::Clock clock;
	while (window.isOpen())
	{
		const float dt = clock.restart().asSeconds();
		handleEvents();
		update(dt);
		draw();
	}
}

void PongGame::handleEvents()
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		if (const auto *key = event->getIf<sf::Event::KeyPressed>())
		{
			if (key->code == sf::Keyboard::Key::Escape)
			{
				window.close();
			}
			if (key->code == sf::Keyboard::Key::Enter)
			{
				if (state == State::Start)
				{
					state = State::Play;
				}
				else
				{
					state = State::Start;
					ball.reset();
				}
			}
		}
	}
}

void PongGame::update(float dt)
{
	player1.dy = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		player1.dy -= PADDLE_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		player1.dy += PADDLE_SPEED;
	}

	player2.dy = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		player2.dy -= PADDLE_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		player2.dy += PADDLE_SPEED;
	}

	player1.update(dt);
	player2.update(dt);
	if (state == State::Play)
	{
		ball.update(dt);
	}
}

void PongGame::draw()
{
	window.clear(BG);
	player1.render(window);
	player2.render(window);
	ball.render(window);
	window.draw(scoreLeft);
	window.draw(scoreRight);
	window.display();
}