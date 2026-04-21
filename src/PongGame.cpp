#include "Constants.hpp"
#include "PongGame.hpp"
#include "Random.hpp"
#include <optional>

constexpr float PADDLE_SPEED = 200.f;
const sf::Color BG(40, 45, 52);

PongGame::PongGame()
	: window(sf::VideoMode({1280u, 720u}), "Pong"), font("assets/font.ttf"), scoreLeft(font, "0", 32),
	  scoreRight(font, "0", 32), fpsText(font, "FPS: 0", 8),
	  player1(10.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f),
	  player2(Constants::VIRTUAL_WIDTH - 15.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f),
	  ball(Constants::VIRTUAL_WIDTH / 2.f - 2, Constants::VIRTUAL_HEIGHT / 2.f - 2, 4.f, 4.f)
{
	window.setVerticalSyncEnabled(true);
	sf::View view(sf::FloatRect({0.f, 0.f}, {Constants::VIRTUAL_WIDTH, Constants::VIRTUAL_HEIGHT}));
	window.setView(view);
	font.setSmooth(false);
	scoreLeft.setPosition({Constants::VIRTUAL_WIDTH / 2.f - 50, 30.f});
	scoreRight.setPosition({Constants::VIRTUAL_WIDTH / 2.f + 30, 30.f});
	fpsText.setFillColor(sf::Color::Green);
	fpsText.setPosition({5.f, 5.f});
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
					ball.serve();
					state = State::Play;
				}
				else
				{
					ball.reset();
					state = State::Start;
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

		std::uniform_real_distribution<float> bounceDY(10.f, 150.f);

		if (ball.bounds().findIntersection(player1.bounds()).has_value())
		{
			ball.dx = -ball.dx * 1.03f;
			ball.x = player1.x + player1.width;
			ball.dy = (ball.dy < 0 ? -1 : 1) * bounceDY(Random::rng());
		}
		if (ball.bounds().findIntersection(player2.bounds()).has_value())
		{
			ball.dx = -ball.dx * 1.03f;
			ball.x = player2.x - ball.width;
			ball.dy = (ball.dy < 0 ? -1 : 1) * bounceDY(Random::rng());
		}

		if (ball.y <= 0)
		{
			ball.y = 0;
			ball.dy = -ball.dy;
		}

		if (ball.y + ball.height >= Constants::VIRTUAL_HEIGHT)
		{
			ball.y = Constants::VIRTUAL_HEIGHT - ball.height;
			ball.dy = -ball.dy;
		}
	}

	fpsFrames++;
	if (fpsClock.getElapsedTime().asSeconds() >= 1.f)
	{
		fpsValue = fpsFrames;
		fpsFrames = 0;
		fpsClock.restart();
		fpsText.setString("FPS: " + std::to_string(fpsValue));
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
	window.draw(fpsText);
	window.display();
}