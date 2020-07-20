#include "async_chat_server.hpp"

#include "chat_session.hpp"

void AsyncChatServer::run()
{
    async_accept();
    io_service.run();
}

void AsyncChatServer::publish(std::string const& message,
    session_ptr publisher = nullptr)
{
    for(auto& client : clients)
    {
        if (client != publisher && client->is_registered())
            client->publish("[" + publisher->get_username() +
                "] " + message + "\n");
    }
}

void AsyncChatServer::async_accept()
{
    auto new_client = std::make_shared<ChatSession>(io_service);
 
    acceptor.async_accept(new_client->get_socket(),
        std::bind(&AsyncChatServer::on_accept, this, new_client,
            std::placeholders::_1));
}

void AsyncChatServer::on_accept(session_ptr client, const error_code error)
{
    if (!error)
    {
        clients.insert(client);

        client->start(
            std::bind(&AsyncChatServer::publish, this,
                std::placeholders::_1,
                std::placeholders::_2),
            [&, client]
            {
                if(clients.erase(client))
                {
                    publish("--- Left chat room ---", client);
                }
            }
        );
    }

    async_accept();
}
