#include "connection.hpp"
#include <iostream>
#include <string>
#include <vector>

constexpr short PORT = 1234;

int main() {
    Connection receiver;
    if (!receiver.Connect("", PORT)) {
        return EXIT_FAILURE;
    }

    std::cout << "Receiver waiting for data..." << std::endl;

    while (true) {
        std::vector<char> data = receiver.Receive();
        if (!data.empty()) {
            std::cout << "Received: " << std::string(data.begin(), data.end()) << std::endl;
        }
    }

    return EXIT_SUCCESS;
}