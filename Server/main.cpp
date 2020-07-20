#include "async_chat_server.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chatserver <ip> <port>" << std::endl;
            return 1;
        }

        io::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        AsyncChatServer chat_server(*iterator);
        chat_server.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}