#include <array>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <asio.hpp>

using asio::ip::udp;


const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%Y-%m-%d %X]", &tstruct);

    return buf;
}


class udp_server
{
public:
    udp_server(asio::io_context& io_context, asio::ip::address ip, int port)
            : socket_(io_context, udp::endpoint(ip, port))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(
                asio::buffer(recv_buffer_), remote_endpoint_,
                std::bind(&udp_server::handle_receive, this,
                          asio::placeholders::error,
                          asio::placeholders::bytes_transferred));
    }

    void handle_receive(const std::error_code& error,
                        std::size_t len)
    {
        if (!error)
        {
            if (len != 1) {
                std::cout << currentDateTime() << " Received: ";
                std::cout.write(recv_buffer_.data(), len);
                std::cout.flush();
                start_receive();
            } else {
                socket_.close();
                std::cout.flush();
            }
        }
    }
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 256> recv_buffer_;
};


int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            std::cout << "Usage: udp_server <address> <port>" << std::endl;
            return 0;
        }
        if (argc != 3) {
            std::cerr << "wrong number of argument required: 2 was given: " << argc - 1 << std::endl;
            return 1;
        }


        asio::ip::address ip;
        int port;

        try {
            ip = asio::ip::address().from_string(argv[1]);
            port = std::stoi(argv[2]);
        } catch (std::exception &e) {
            std::cout << "Exception: Invalid argument" << std::endl;
            return 0;
        }

        asio::io_context io_context;
        udp_server server(io_context, ip, port);
        io_context.run();

    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
