#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <enet/enet.h>
#include <string>
#include <vector>

class Connection {
public:
    Connection();
    ~Connection();

    // For a server, pass an empty address string and the port to listen on.
    // For a client, pass the server address and port to connect to.
    bool Connect(const std::string& address, enet_uint16 port);

    void Disconnect();

    // For a client, sends data to the server peer.
    // For a server, broadcasts to all connected peers.
    void Send(const std::vector<char>& data);

    // Blocking call that waits for and returns a datagram.
    // Handles connect/disconnect events internally.
    // Returns an empty vector on error or if Disconnect is called.
    std::vector<char> Receive();

private:
    ENetHost* m_host = nullptr;
    ENetPeer* m_peer = nullptr; // For a client, this is the connection to the server.
    bool m_is_server = false;
    bool m_disconnecting = false;
};

#endif // CONNECTION_HPP