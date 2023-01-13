#ifndef NETWORK_H
#define NETWORK_H

#include <gf/Packet.h>
#include <gf/Queue.h>
#include <gf/TcpSocket.h>
#include <gf/Log.h>
#include <gf/SerializationOps.h>

#include <atomic>
#include <mutex>
#include <string>
#include <stdexcept>
#include <thread>

class Network {
    public:
        gf::Queue<gf::Packet> m_queue;

    public:
        Network();

        bool isConnected();
        void connect(const std::string& hostname, const std::string& port);
        void deconnect();

        template<typename T>
        void send(const T& data) {
            gf::Packet packet;
            packet.is(data);
            m_socket.sendPacket(packet);
        }

    private:
        gf::TcpSocket m_socket;
        std::mutex m_mutex;
        std::atomic_bool m_connecting;

    private:
        void run(std::string hostname, std::string port);
};

#endif // NETWORK_H
