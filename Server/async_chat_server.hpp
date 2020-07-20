#ifndef ASYNC_CHAT_SERVER_H
#define ASYNC_CHAT_SERVER_H

#include "async_common.hpp"

#include <unordered_set>
#include <memory>

class ChatSession;

using session_ptr = std::shared_ptr<ChatSession>;

class AsyncChatServer
{
public:
    AsyncChatServer(const tcp::endpoint& ep) : acceptor(io_service, ep) { };
    void run();
    void publish(std::string const& message, session_ptr publisher);

private:
    void async_accept();
    void on_accept(session_ptr client, const error_code error);

private:
    std::unordered_set<session_ptr> clients;
    io::io_service io_service;
    tcp::acceptor acceptor;
};

#endif // ASYNC_CHAT_SERVER_H