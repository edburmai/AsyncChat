
#include "async_chat_client.hpp"

#include <thread>
#include <functional>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chatclient <host> <port>" << std::endl;
            return 1;
        }

        io::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        AsyncChatClient client(io_service, *iterator);

        // Run IO in a separate thread which will handle async read/write requests
        std::thread control_thread([&](){
            io_service.run();
        });

        std::cout << "Enter your nickname: ";

        // Register client with a nickname
        std::string message;
        std::cin >> message;
        char login_code = REGISTER_MSG_CODE;
        message = login_code + message;
        client.send_message(message);

        // Message sending loop
        while (std::getline(std::cin, message))
        {
            client.send_message(message);
        }

        control_thread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}