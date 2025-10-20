#include "connection.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

constexpr const char* BROADCAST_ADDRESS = "255.255.255.255";
constexpr short BROADCAST_PORT = 30001;

int main() {
    Connection sender;
    if (!sender.Connect(BROADCAST_ADDRESS, BROADCAST_PORT)) {
        return EXIT_FAILURE;
    }

    int count = 0;
    auto last_send_time = std::chrono::steady_clock::now();

    while (true) {
        // Non-blocking receive to process any incoming packets
        sender.Receive();

        auto now = std::chrono::steady_clock::now();
        if (now - last_send_time >= std::chrono::seconds(1)) {
            std::string message = "hello " + std::to_string(count++);
            sender.Send(message.c_str(), message.length() + 1);
            std::cout << "Sent packet: " << message << std::endl;
            last_send_time = now;
        }
    }

    sender.Disconnect();
    return EXIT_SUCCESS;
}