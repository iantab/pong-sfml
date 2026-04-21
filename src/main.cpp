#include <SFML/Graphics.hpp>
#include <algorithm>
#include <optional>
#include <random>

constexpr unsigned VIRTUAL_WIDTH = 432;
constexpr unsigned VIRTUAL_HEIGHT = 243;

constexpr float PADDLE_SPEED = 200.f;
constexpr float PADDLE_HEIGHT = 20.f;

enum class GameState { Start, Play };

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1280u, 720u }), "Pong");
	window.setVerticalSyncEnabled(true);

	sf::View view(sf::FloatRect({ 0.f, 0.f }, { (float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT }));
	window.setView(view);

	const sf::Color BG(40, 45, 52);

	sf::Font font("assets/font.ttf");
	font.setSmooth(false);

	sf::Text scoreLeft(font, "0", 32);
	sf::Text scoreRight(font, "0", 32);
	scoreLeft.setPosition({ VIRTUAL_WIDTH / 2.f - 50, 30.f });
	scoreRight.setPosition({ VIRTUAL_WIDTH / 2.f + 30, 30.f });

	sf::RectangleShape leftPaddle({ 5.f, 20.f });
	leftPaddle.setPosition({ 10.f, VIRTUAL_HEIGHT / 2.f - 10 });

	sf::RectangleShape rightPaddle({ 5.f, 20.f });
	rightPaddle.setPosition({ VIRTUAL_WIDTH - 15.f, VIRTUAL_HEIGHT / 2.f - 10 });

	sf::RectangleShape ball({ 4.f, 4.f });
	ball.setPosition({ VIRTUAL_WIDTH / 2.f - 2, VIRTUAL_HEIGHT / 2.f - 2 });

	float leftY = VIRTUAL_HEIGHT / 2.f - 10;
	float rightY = VIRTUAL_HEIGHT / 2.f - 10;
	float ballX = VIRTUAL_WIDTH / 2.f - 2;
	float ballY = VIRTUAL_HEIGHT / 2.f - 2;
	float ballDX = 0.f, ballDY = 0.f;

	GameState state = GameState::Start;

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> randDY(-50.f, 50.f);

	auto resetBall = [&]() {
		ballX = VIRTUAL_WIDTH / 2.f - 2;
		ballY = VIRTUAL_HEIGHT / 2.f - 2;
		ballDX = (rng() % 2 == 0) ? 100.f : -100.f;
		ballDY = randDY(rng);
		};
	resetBall();

	sf::Clock clock;

	while (window.isOpen()) {

		const float dt = clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
				if (key->code == sf::Keyboard::Key::Escape) {
					window.close();
				}
				if (key->code == sf::Keyboard::Key::Enter) {
					if (state == GameState::Start) {
						state = GameState::Play;
					}
					else {
						state = GameState::Start;
						resetBall();
					}
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			leftY -= PADDLE_SPEED * dt;
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			leftY += PADDLE_SPEED * dt;
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			rightY -= PADDLE_SPEED * dt;
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			rightY += PADDLE_SPEED * dt;
		}

		leftY = std::clamp(leftY, 0.f, VIRTUAL_HEIGHT - PADDLE_HEIGHT);
		rightY = std::clamp(rightY, 0.f, VIRTUAL_HEIGHT - PADDLE_HEIGHT);

		if (state == GameState::Play) {
			ballX += ballDX * dt;
			ballY += ballDY * dt;
		}

		leftPaddle.setPosition({ 10.f, leftY });
		rightPaddle.setPosition({ VIRTUAL_WIDTH - 15.f, rightY });
		ball.setPosition({ ballX, ballY });

		window.clear(BG);
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);
		window.draw(scoreLeft);
		window.draw(scoreRight);
		window.display();
	}
}