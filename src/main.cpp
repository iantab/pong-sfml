#include "Ball.hpp"
#include "Constants.hpp"
#include "Paddle.hpp"
#include "Player.hpp"
#include "Random.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <random>
#include <string>

enum class State
{
	Start,
	Serve,
	Play,
	Done
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

		sf::Text largeMessage(font, "", 16);
		sf::Text smallMessage(font, "", 8);

		sf::SoundBuffer paddleHitBuf("assets/paddle_hit.wav"), wallHitBuf("assets/wall_hit.wav"),
			scoreBuf("assets/score.wav");
		sf::Sound paddleHit(paddleHitBuf), wallHit(wallHitBuf), score(scoreBuf);

		Paddle player1(10.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f);
		Paddle player2(Constants::VIRTUAL_WIDTH - 15.f, Constants::VIRTUAL_HEIGHT / 2.f - 10, 5.f, 20.f);
		Ball ball(Constants::VIRTUAL_WIDTH / 2.f - 2, Constants::VIRTUAL_HEIGHT / 2.f - 2, 4.f, 4.f);

		State state = State::Start;
		sf::Clock fpsClock;
		int fpsFrames = 0;
		int fpsValue = 0;
		int player1Score = 0;
		int player2Score = 0;
		Player servingPlayer = Player::One;
		Player winningPlayer = Player::Neither;

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
						switch (state)
						{
						case State::Start:
							state = State::Serve;
							break;
						case State::Serve:
							ball.reset(servingPlayer);
							state = State::Play;
							break;
						case State::Play:
							break;
						case State::Done:
							state = State::Serve;
							player1Score = 0;
							player2Score = 0;
							servingPlayer = (winningPlayer == Player::One) ? Player::Two : Player::One;
							ball.reset(servingPlayer);
							break;
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
					paddleHit.play();
				}
				if (ball.bounds().findIntersection(player2.bounds()).has_value())
				{
					ball.dx = -ball.dx * 1.03f;
					ball.x = player2.x - ball.width;
					ball.dy = (ball.dy < 0 ? -1 : 1) * bounceDY(Random::rng());
					paddleHit.play();
				}

				if (ball.y <= 0)
				{
					ball.y = 0;
					ball.dy = -ball.dy;
					wallHit.play();
				}

				if (ball.y + ball.height >= Constants::VIRTUAL_HEIGHT)
				{
					ball.y = Constants::VIRTUAL_HEIGHT - ball.height;
					ball.dy = -ball.dy;
					wallHit.play();
				}

				if (ball.x < 0)
				{
					player2Score++;
					score.play();
					if (player2Score == Constants::WIN_SCORE)
					{
						winningPlayer = Player::Two;
						state = State::Done;
					}
					else
					{
						servingPlayer = Player::One;
						ball.reset(servingPlayer);
						state = State::Serve;
					}
				}
				if (ball.x > Constants::VIRTUAL_WIDTH)
				{

					player1Score++;
					score.play();
					if (player1Score == Constants::WIN_SCORE)
					{
						winningPlayer = Player::One;
						state = State::Done;
					}
					else
					{
						servingPlayer = Player::Two;
						ball.reset(servingPlayer);
						state = State::Serve;
					}
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

			bool showMessages = true;
			switch (state)
			{
			case State::Start:
				largeMessage.setString("Welcome to Pong!");
				smallMessage.setString("Press Enter to begin");
				break;
			case State::Serve:
				largeMessage.setString("Player " + std::to_string(static_cast<int>(servingPlayer)) + "'s serve!");
				smallMessage.setString("Press Enter to serve");
				break;
			case State::Play:
				showMessages = false;
				break;
			case State::Done:
				largeMessage.setString("Player " + std::to_string(static_cast<int>(winningPlayer)) + " wins!");
				smallMessage.setString("Press Enter to restart");
				break;
			}
			if (showMessages)
			{
				largeMessage.setPosition(
					{(Constants::VIRTUAL_WIDTH - largeMessage.getLocalBounds().size.x) / 2.f, 9.f});
				smallMessage.setPosition(
					{(Constants::VIRTUAL_WIDTH - smallMessage.getLocalBounds().size.x) / 2.f, 27.f});
				window.draw(largeMessage);
				window.draw(smallMessage);
			}
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
