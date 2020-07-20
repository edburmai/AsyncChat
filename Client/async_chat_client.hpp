#ifndef ASYNC_CHAT_CLIENT
#define ASYNC_CHAT_CLIENT

#include "async_common.hpp"

#include <queue>

class AsyncChatClient
{
public:
    AsyncChatClient(io::io_service& io_service, const tcp::endpoint& ep);
    void send_message(const std::string& message);

private:
    void on_connect(const error_code& error);

    void async_read();
    void on_read(const error_code& error, const std::size_t bytes_transferred);

    void send_message_impl(const std::string& message);
    void async_write();
    void on_write(const error_code& error, const std::size_t bytes_transferred);

private:
    io::io_service& io_service;
    tcp::socket socket;
    std::queue<std::string> outgoing;
    char read_buffer[MAX_MSG_LEN];

};

#endif // ASYNC_CHAT_CLIENT