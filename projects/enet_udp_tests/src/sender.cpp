#include "connection.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

constexpr const char* MULTICAST_ADDRESS = "239.255.0.1";
constexpr short MULTICAST_PORT = 30001;

int main() {
    Connection sender;
    if (!sender.Connect(MULTICAST_ADDRESS, MULTICAST_PORT)) {
        return EXIT_FAILURE;
    }

    int count = 0;
    while (true) {
        std::string message = "hello " + std::to_string(count++);
        sender.Send(message.c_str(), message.length() + 1);
        std::cout << "Sent packet: " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    sender.Disconnect();
    return EXIT_SUCCESS;
}