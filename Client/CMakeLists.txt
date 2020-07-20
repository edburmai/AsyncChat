cmake_minimum_required(VERSION 3.0)

project(ChatClient)

find_package(Boost 1.65.0 COMPONENTS system thread REQUIRED)

set(chat_client_source_files
  async_chat_client.cpp
  async_chat_client.hpp
  main.cpp
)

add_executable(chatclient ${chat_client_source_files})
target_link_libraries(chatclient ${Boost_LIBRARIES})
target_include_directories(chatclient PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(chatclient PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
