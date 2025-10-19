#include "connection.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

constexpr const char* SERVER_ADDRESS = "127.0.0.1";
constexpr short PORT = 1234;

int main() {
    Connection sender;
    if (!sender.Connect(SERVER_ADDRESS, PORT)) {
        return EXIT_FAILURE;
    }

    std::string message_str = "Hello from ENet!";
    std::vector<char> message(message_str.begin(), message_str.end());
    while (1){
        sender.Send(message);
        std::cout << "Sent packet: \"" << message_str << "\"" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sender.Disconnect();
    return EXIT_SUCCESS;
}