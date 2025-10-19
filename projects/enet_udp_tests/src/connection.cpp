#include "connection.hpp"
#include <iostream>

Connection::Connection() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }
}

Connection::~Connection() {
    Disconnect();
    enet_deinitialize();
}

bool Connection::Connect(const std::string& address_str, enet_uint16 port) {
    m_disconnecting = false;
    if (address_str.empty()) { // Server
        m_is_server = true;
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;
        m_host = enet_host_create(&address, 32, 1, 0, 0);
        if (m_host == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
            return false;
        }
    } else { // Client
        m_is_server = false;
        m_host = enet_host_create(nullptr, 1, 1, 0, 0);
        if (m_host == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
            return false;
        }

        ENetAddress address;
        enet_address_set_host(&address, address_str.c_str());
        address.port = port;

        m_peer = enet_host_connect(m_host, &address, 1, 0);
        if (m_peer == nullptr) {
            std::cerr << "No available peers for initiating an ENet connection." << std::endl;
            return false;
        }

        ENetEvent event;
        if (enet_host_service(m_host, &event, 5000) <= 0 || event.type != ENET_EVENT_TYPE_CONNECT) {
            enet_peer_reset(m_peer);
            m_peer = nullptr;
            std::cerr << "Connection to " << address_str << ":" << port << " failed." << std::endl;
            return false;
        }
    }
    return true;
}

void Connection::Disconnect() {
    m_disconnecting = true;
    if (m_peer != nullptr) {
        enet_peer_disconnect(m_peer, 0);
        ENetEvent event;
        while(enet_host_service(m_host, &event, 1000) > 0) {
            if (event.type == ENET_EVENT_TYPE_DISCONNECT) break;
        }
        enet_peer_reset(m_peer);
        m_peer = nullptr;
    }
    if (m_host != nullptr) {
        enet_host_destroy(m_host);
        m_host = nullptr;
    }
}

void Connection::Send(const std::vector<char>& data) {
    if (m_host == nullptr || data.empty()) return;

    ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    if (m_is_server) {
        enet_host_broadcast(m_host, 0, packet);
    } else if (m_peer != nullptr) {
        enet_peer_send(m_peer, 0, packet);
    } else {
        enet_packet_destroy(packet);
    }
}

std::vector<char> Connection::Receive() {
    if (m_host == nullptr) return {};

    while (!m_disconnecting) {
        ENetEvent event;
        int service_result = enet_host_service(m_host, &event, 1000); // 1 second timeout
        if (service_result > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::vector<char> data(event.packet->data, event.packet->data + event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    return data; // Got data, return to user
                }
                case ENET_EVENT_TYPE_CONNECT:
                    printf("Client connected: %x:%u\n", event.peer->address.host, event.peer->address.port);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Client disconnected.\n");
                    if (!m_is_server && event.peer == m_peer) {
                        m_peer = nullptr;
                    }
                    break;
                default:
                    break;
            }
        } else if (service_result < 0) {
            // Error
            return {};
        }
        // if timeout, loop again
    }
    return {}; // Disconnecting
}