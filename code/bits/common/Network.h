#ifndef TSL_NETWORK_H
#define TSL_NETWORK_H

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

namespace tsl {

    class Network {
        public:
            gf::Queue<gf::Packet> m_queue;

            Network();

            bool isConnecting();
            bool isConnected();
            void connect(const std::string& hostname);
            void disconnect();

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

            void run(std::string hostname);
    };

}

#endif // TSL_NETWORK_H