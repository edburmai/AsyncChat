#include "chat_session.hpp"

void ChatSession::publish(std::string const& message)
{
    // if the queue is not empty, write request was already scheduled.
    // Just push a message to the queue.
    bool write_in_progress = !outgoing.empty();
    outgoing.push(message);

    if(!write_in_progress)
        async_write();
}

void ChatSession::async_write()
{
    socket.async_write_some(io::buffer(outgoing.front()),
        std::bind(&ChatSession::on_write, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void ChatSession::on_write(const error_code& error, std::size_t bytes_transferred)
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
        on_error();
    }
}

void ChatSession::start(message_handler&& publish_cb, error_handler&& error_cb)
{
    publish_for_all = publish_cb;
    on_error = error_cb;
    async_read();
}

void ChatSession::async_read()
{
    socket.async_read_some(io::buffer(read_buffer),
        std::bind(&ChatSession::on_read, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void ChatSession::on_read(const error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        std::string msg(read_buffer, bytes_transferred);

        if (read_buffer[0] == REGISTER_MSG_CODE && !is_registered())
        {
            username = std::move(msg.substr(1));
            publish("[" + username + ", welcome to chat]\n");
            publish_for_all(" --- New user joined ---", shared_from_this());
        }
        else if (is_registered())
        {
            publish_for_all(msg, shared_from_this());
        }

        async_read();
    }
    else
    {
        socket.close();
        on_error();
    }
}