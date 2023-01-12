#include "Network.hpp"

Network::Network() {

}

bool Network::isConnected() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return static_cast<bool>(m_socket);
}

void Network::connect(const std::string& hostname, const std::string& port) {

    std::thread thread(&Network::run, this, hostname, port);
    thread.detach();
}

void::Network::deconnect() {
    m_socket.~TcpSocket();
    m_socket = gf::TcpSocket();
    m_queue.clear();
}

void Network::run(std::string hostname, std::string port) {
    gf::TcpSocket socket(hostname, port);

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

