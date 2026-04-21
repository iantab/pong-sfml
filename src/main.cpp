#include "Ball.hpp"
#include "Constants.hpp"
#include "Paddle.hpp"
#include "Random.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <random>
#include <string>

enum class State
{
	Start,
	Serve,
	Play
};

int main()
{
	try
	{
		const sf::Color BG(40, 45, 52);

		sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), "Pong");
		window.setVerticalSyncEnabled(true);
		sf::View view(sf::FloatRect({0.f, 0.f}, {Constants::VIRTUAL_WIDTH, Constants::VIRTUAL_HEIGHT}));
		window.setView(view);

		sf::Font font("assets/font.ttf");
		font.setSmooth(false);

		sf::Text scoreLeft(font, "0", 32);
		scoreLeft.setPosition({Constants::VIRTUAL_WIDTH / 2.f - 50, 30.f});

		sf::Text scoreRight(font, "0", 32);
		scoreRight.setPosition({Constants::VIRTUAL_WIDTH / 2.f + 30, 30.f});

		sf::Text fpsText(font, "FPS: 0", 8);
		fpsText.setFillColor(sf::Color::Green);
		fpsText.setPosition({5.f, 5.f});

		Paddle player1(10.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f);
		Paddle player2(Constants::VIRTUAL_WIDTH - 15.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f);
		Ball ball(Constants::VIRTUAL_WIDTH / 2.f - 2, Constants::VIRTUAL_HEIGHT / 2.f - 2, 4.f, 4.f);

		State state = State::Start;
		sf::Clock fpsClock;
		int fpsFrames = 0;
		int fpsValue = 0;
		int player1Score = 0;
		int player2Score = 0;

		auto handleEvents = [&] {
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
						if (state == State::Start || state == State::Serve)
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
		};

		auto update = [&](float dt) {
			player1.dy = 0.f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				player1.dy -= Constants::PADDLE_SPEED;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				player1.dy += Constants::PADDLE_SPEED;
			}

			player2.dy = 0.f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				player2.dy -= Constants::PADDLE_SPEED;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				player2.dy += Constants::PADDLE_SPEED;
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

				if (ball.x < 0)
				{
					player2Score++;
					ball.reset();
					state = State::Serve;
				}
				if (ball.x > Constants::VIRTUAL_WIDTH)
				{
					player1Score++;
					ball.reset();
					state = State::Serve;
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
		};

		auto draw = [&] {
			window.clear(BG);
			player1.render(window);
			player2.render(window);
			ball.render(window);
			scoreLeft.setString(std::to_string(player1Score));
			scoreRight.setString(std::to_string(player2Score));
			window.draw(scoreLeft);
			window.draw(scoreRight);
			window.draw(fpsText);
			window.display();
		};

		sf::Clock clock;
		while (window.isOpen())
		{
			const float dt = clock.restart().asSeconds();
			handleEvents();
			update(dt);
			draw();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Fatal: " << e.what() << '\n';
		return 1;
	}
}
