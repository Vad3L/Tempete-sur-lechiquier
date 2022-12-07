#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>
#include <gf/Grid.h>
#include <gf/View.h>
#include <gf/Rect.h>

struct Game {
	gf::Window* window;
	Game(gf::Window& w) {
		window = &w;
	}
};

// gère les inptus
void inputs (Game& g, gf::Event& event) {
	if (event.type == gf::EventType::Closed) {
		g.window->close();
	} else if (event.type == gf::EventType::KeyPressed || event.type == gf::EventType::KeyRepeated) {
		if (event.key.keycode == gf::Keycode::Q) {
			g.window->close();
		}
	}
}
 
int main() {
	gf::Window window("Example", { 640, 480 });
	gf::RenderWindow renderer(window);
	Game g(window);

	while (window.isOpen()) {
		gf::Event event;
		while (window.pollEvent(event)) {
			inputs(g, event);
		}

		renderer.clear();
		renderer.display();
	}

	return 0;
}

