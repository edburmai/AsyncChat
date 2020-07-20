#include "async_chat_client.hpp"

#include <iostream>

AsyncChatClient::AsyncChatClient(io::io_service& io_service,
    const tcp::endpoint& ep) : io_service(io_service), socket(io_service)
{
    socket.async_connect(ep,
        std::bind(&AsyncChatClient::on_connect, this,
          std::placeholders::_1));
}

void AsyncChatClient::on_connect(const error_code& error)
{
    if (!error)
        async_read();
    else
        std::cerr << "Failed to connect" << std::endl;
}

void AsyncChatClient::async_read()
{
    socket.async_read_some(io::buffer(read_buffer),
        std::bind(&AsyncChatClient::on_read, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void AsyncChatClient::on_read(const error_code& error, const std::size_t bytes_transferred)
{
    if (!error)
    {
        std::string msg(read_buffer, bytes_transferred);
        std::cout << msg;
        async_read();
    }
    else
    {
        socket.close();
    }
}

void AsyncChatClient::send_message(const std::string& message)
{
    io_service.post(std::bind(&AsyncChatClient::send_message_impl, this, message));
}

void AsyncChatClient::send_message_impl(const std::string& message)
{
    bool write_in_progress = !outgoing.empty();
    outgoing.push(message);

    if (!write_in_progress)
      async_write();
}

void AsyncChatClient::async_write()
{
    socket.async_write_some(io::buffer(outgoing.front()),
        std::bind(&AsyncChatClient::on_write, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void AsyncChatClient::on_write(const error_code& error, const std::size_t bytes_transferred)
{
    if(!error)
    {
        outgoing.pop();

        if(!outgoing.empty())
        {
            async_write();
        }
    }
    else
    {
        socket.close();
    }
}
