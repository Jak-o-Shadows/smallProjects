#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <asio.hpp>
#include <string>
#include <vector>

class Connection {
public:
    // Connect joins the multicast group.
    bool Connect(const std::string& multicast_address, short multicast_port);

    // Disconnect leaves the group.
    void Disconnect();

    // Send multicasts a datagram to the group.
    void Send(const char* data, size_t data_length);

    // Receive waits for and returns a datagram from the group.
    std::vector<char> Receive();

private:
    asio::io_context m_io_context;
    asio::ip::udp::socket m_socket{m_io_context};
    asio::ip::udp::endpoint m_endpoint;
};

#endif // CONNECTION_HPP