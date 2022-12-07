#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Shapes.h>
#include <gf/Color.h>
#include <gf/TcpSocket.h>
#include <gf/Packet.h>

#include "../protocole/protocole.h"

#include <iostream>

struct Game {
	gf::Window* window;
	gf::RectangleShape r;
	gf::TcpSocket s;
	bool connected = false;
	Game(gf::Window& w) {
		window = &w;
		r.setSize({ 100.f, 100.f });
		r.setPosition({ 50.f, 50.f });
		r.setColor(gf::Color::Red);
		std::string port = "4545";
		std::string host = "127.0.0.1";
		s = gf::TcpSocket(host, port);
		if (!s) {
			std::cerr << "Erreur de connexion au serveur" << std::endl;
		} else {
			connected = true;
		}
	}

	int recv_packet () {
		gf::Packet paquet;
		if (gf::SocketStatus::Data != s.recvPacket(paquet)) {
			std::cerr << "Erreur lors de la réception du paquet" << std::endl;
			return -1;
		}
		auto req = paquet.as<Paquet>();
		switch (req.choix) {
			case 1:
				r.setPosition({ 50.f, 50.f });
				break;
			case 2:
				r.setPosition({ 100.f, 100.f });
		}
		return 0;
	}
};

// gère les inptus
void inputs (Game& g, gf::Event& event) {
	if (event.type == gf::EventType::Closed || !g.connected) {
		g.window->close();
	} else if (event.type == gf::EventType::KeyPressed || event.type == gf::EventType::KeyRepeated) {
		if (event.key.keycode == gf::Keycode::D) {
			gf::Packet paquet;
			Paquet p;
			p.choix = 1;
			paquet.is(p);
			g.s.sendPacket(paquet);
			g.recv_packet();
		}
		if (event.key.keycode == gf::Keycode::S) {
			gf::Packet paquet;
			Paquet p;
			p.choix = 2;
			paquet.is(p);
			g.s.sendPacket(paquet);
			g.recv_packet();
		}
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
		renderer.draw(g.r);
		renderer.display();
	}

	return 0;
}

