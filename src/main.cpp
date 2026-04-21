#include <SFML/Graphics.hpp>
#include <optional>

constexpr unsigned VIRTUAL_WIDTH = 432;
constexpr unsigned VIRTUAL_HEIGHT = 243;

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1280u, 720u }), "Pong");
	window.setVerticalSyncEnabled(true);

	sf::View view(sf::FloatRect({ 0.f, 0.f }, { (float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT }));
	window.setView(view);

	sf::Font font("assets/font.ttf");
	font.setSmooth(false);

	sf::Text text(font, "Hello Pong", 8);
	text.setFillColor(sf::Color::White);
	text.setPosition({ VIRTUAL_WIDTH / 2.f - 30, VIRTUAL_HEIGHT / 2.f });

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
		window.clear(sf::Color::Black);
		window.draw(text);
		window.display();
	}
}