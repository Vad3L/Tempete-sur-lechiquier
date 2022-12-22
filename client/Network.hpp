#ifndef NETWORK_H
#define NETWORK_H

#include <atomic>
#include <mutex>
#include <string>

#include <gf/Packet.h>
#include <gf/Queue.h>
#include <gf/TcpSocket.h>

class Network {
    public:
        Network();

        bool isConnected();
        void connect(const std::string& hostname, const std::string& port);

        template<typename T>
        void send(const T& data) {
            gf::Packet packet;
            packet.is(data);
            m_socket.sendPacket(packet);
        }

        gf::Queue<gf::Packet> queue;

    private:
        gf::TcpSocket m_socket;
        std::mutex m_mutex;
        std::atomic_bool m_connecting;

        void run(std::string hostname, std::string port);
};

#endif // NETWORK_H