#include "connection.hpp"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* MULTICAST_ADDRESS = "239.255.0.1";
constexpr short MULTICAST_PORT = 30001;

int main() {
    Connection receiver;
    if (!receiver.Connect(MULTICAST_ADDRESS, MULTICAST_PORT)) {
        return EXIT_FAILURE;
    }

    while (true) {
        std::vector<char> data = receiver.Receive();
        std::cout << "Received: " << data.data() << std::endl;
    }

    return EXIT_SUCCESS;
}