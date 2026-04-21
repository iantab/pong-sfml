#include <SFML/Graphics.hpp>
#include <optional>

constexpr unsigned VIRTUAL_WIDTH = 432;
constexpr unsigned VIRTUAL_HEIGHT = 243;

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

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
				if (key->code == sf::Keyboard::Key::Escape) {
					window.close();
				}
			}
		}
		window.clear(BG);
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);
		window.draw(scoreLeft);
		window.draw(scoreRight);
		window.display();
	}
}