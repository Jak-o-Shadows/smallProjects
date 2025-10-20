#include "connection.hpp"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* BROADCAST_ADDRESS = "255.255.255.255";
constexpr short BROADCAST_PORT = 30001;

int main() {
    Connection receiver;
    if (!receiver.Connect(BROADCAST_ADDRESS, BROADCAST_PORT)) {
        return EXIT_FAILURE;
    }

    int rx_count = 0;
    while (true) {
        std::vector<char> data = receiver.Receive();
        if (!data.empty()){
            std::cout << "Received: " << data.data() << std::endl;
            if (std::string(data.data()).find("Ack") == std::string::npos) {
                // If it's not from one we sent (ie. doesnt have Ack in it), reply
                rx_count++;
                std::string msg = "Ack " + std::to_string(rx_count) + " from receiver";
                receiver.Send(msg.c_str(), msg.size());
            }

        }
    }

    return EXIT_SUCCESS;
}