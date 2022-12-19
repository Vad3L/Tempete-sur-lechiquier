#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Shapes.h>
#include <gf/Color.h>
#include <gf/TcpSocket.h>
#include <gf/Packet.h>

#include "../protocole/protocole.h"

#include <iostream>
#include <thread>
#include <mutex>

struct Game {
	gf::Window* window;
	gf::RectangleShape r;
	std::mutex lock;
	gf::TcpSocket s;
	bool connected = false;
	Game(gf::Window& w, std::string host, std::string port) {
		window = &w;
		r.setSize({ 100.f, 100.f });
		r.setPosition({ 50.f, 50.f });
		r.setColor(gf::Color::Red);
		s = gf::TcpSocket(host, port);
		if (!s) {
			std::cerr << "Erreur de connexion au serveur" << std::endl;
		} else {
			connected = true;
		}
	}

	int recv_packet () {
		std::lock_guard<std::mutex> guard(lock);
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
				r.setPosition({ 100.f, 50.f });
		}
		return 0;
	}
};

// gère les inptus
void inputs (Game& g, gf::Event event) {
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
 
int main (int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: ./client [ip] [port]" << std::endl;
		return 0;
	}

	gf::Window window("Example", { 640, 480 });
	gf::RenderWindow renderer(window);
	Game g(window, std::string(argv[1]), std::string(argv[2]));
	// premier paquet pour initialiser le carré
	gf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			std::thread t(inputs, std::ref(g), event);
			t.detach();
		}

		renderer.clear();
		renderer.draw(g.r);
		renderer.display();
	}

	return 0;
}

