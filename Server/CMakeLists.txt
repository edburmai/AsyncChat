cmake_minimum_required(VERSION 3.0)

project(ChatServer)

find_package(Boost 1.65.0 COMPONENTS system thread REQUIRED)

set(chat_server_source_files
  async_chat_server.cpp
  async_chat_server.hpp
  chat_session.cpp
  chat_session.hpp
  main.cpp
)

add_executable(chatserver ${chat_server_source_files})
target_link_libraries(chatserver ${Boost_LIBRARIES})
target_include_directories(chatserver PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(chatserver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
