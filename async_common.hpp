#ifndef ASYNC_COMMON_H
#define ASYNC_COMMON_H

#include <boost/asio.hpp>

#define MAX_MSG_LEN 1024
#define REGISTER_MSG_CODE 0x77

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

#endif // ASYNC_COMMON_H