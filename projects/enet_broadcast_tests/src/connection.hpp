#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <asio.hpp>

#include <string>
#include <vector>

class Connection {
public:
    Connection();

    // Connect joins the boradcast.
    bool Connect(const std::string& broadcast_addr, short broadcast_port);

    // Disconnect leaves the group.
    void Disconnect();

    // Send multicasts a datagram to the group.
    void Send(const char* data, size_t data_length);

    // Receive waits for and returns a datagram from the group.
    std::vector<char> Receive();

private:
    asio::io_context io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint broadcast_endpoint;
    asio::ip::udp::endpoint sender_endpoint;
};

#endif // CONNECTION_HPP