#include <SFML/Graphics.hpp>
#include <optional>

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1280u, 720u }), "Pong");
	window.setVerticalSyncEnabled(true);

	sf::Font font("assets/font.ttf");
	sf::Text text(font, "Hello Pong", 20);

	text.setFillColor(sf::Color::White);
	text.setPosition({ 600.f, 360.f });

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