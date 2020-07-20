#ifndef CHAT_SESSION_H
#define CHAT_SESSION_H

#include "async_common.hpp"

#include <string>
#include <queue>

class ChatSession;

using message_handler = std::function<void (std::string, std::shared_ptr<ChatSession>)>;
using error_handler = std::function<void ()>;

class ChatSession : public std::enable_shared_from_this<ChatSession>
{
public:
    ChatSession(io::io_service& io_service) : socket(io_service) { };

    void publish(const std::string& message);
    void start(message_handler&& on_message, error_handler&& on_error);

    tcp::socket& get_socket() { return socket; }
    const std::string& get_username() { return username; };
    bool is_registered() { return !username.empty(); }

private:
    void async_read();
    void on_read(const error_code& error, std::size_t bytes_transferred);

    void async_write();
    void on_write(const error_code& error, std::size_t bytes_transferred);

private:
    tcp::socket socket;
    std::queue<std::string> outgoing;
    std::string username;

    message_handler publish_for_all;
    error_handler on_error;

    char read_buffer[MAX_MSG_LEN];
};

#endif // CHAT_SESSION_H