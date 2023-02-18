#include "Network.h"

#include "Constants.h"

namespace tsl {

	Network::Network() {

	}

	bool Network::isConnecting() {
    	return m_connecting;
  	}

	bool Network::isConnected() {
		std::lock_guard<std::mutex> guard(m_mutex);
		return static_cast<bool>(m_socket);
	}

	void Network::connect(const std::string& hostname) {
		m_connecting = true;
		
		std::thread thread(&Network::run, this, hostname);
		thread.detach();
	}

	void Network::disconnect() {
		m_queue.clear();
		m_socket = gf::TcpSocket();
		m_connecting = false;
	}

	void Network::run(std::string hostname) {
		gf::TcpSocket socket(hostname, Service);

		if (!socket) {
			m_connecting = false;
			return;
		}

		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_socket = std::move(socket);
		}

		for (;;) {
			gf::Packet packet;

			switch (m_socket.recvPacket(packet)) {
			case gf::SocketStatus::Data:
				m_queue.push(std::move(packet));
				break;
			case gf::SocketStatus::Error:
				gf::Log::error("Lors de la réception du paquet venant du serveur\n");
				return;
			case gf::SocketStatus::Close:
				gf::Log::info("Fin de connection au serveur\n");
				return;
			case gf::SocketStatus::Block:
				assert(false);
				break;
			}
		}
	}

}