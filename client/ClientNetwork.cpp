#include "ClientNetwork.hpp"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>


ClientNetwork::ClientNetwork()
    : m_connecting(false)
{

}


bool ClientNetwork::isConnecting() {
    return m_connecting;
}

bool ClientNetwork::isConnected() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return static_cast<bool>(m_socket);
}

void ClientNetwork::connect(const std::string& hostname, const std::string& port) {
    m_connecting = true;

    std::thread thread(&ClientNetwork::run, this, hostname, port);
    thread.detach();

}


void ClientNetwork::run(std::string hostname, std::string port) {
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
            queue.push(std::move(packet));
            break;
        case gf::SocketStatus::Error:
            gf::Log::error("Error while receiving a packet from server\n");
            return;
        case gf::SocketStatus::Close:
            gf::Log::info("End of connection to the server\n");
            return;
        case gf::SocketStatus::Block:
            assert(false);
            break;
        }
    }
}
