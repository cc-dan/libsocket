#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <netinet/in.h>
#include <cstdint>

namespace network
{
    typedef std::uint16_t port;

    class socket
    {
    public:
        socket();
        ~socket();
        void open(port local);
        void listen(unsigned limit) const;
        socket accept() const;
        void connect(const std::string &ip, network::port remote);
        void send(const char *data, unsigned size) const;
        void send(const std::string &expression) const;
        int receive(char *buffer, unsigned size) const;
        std::string receive(unsigned quota) const;
        int get_fd() const;
        void shutdown() const;
    private:
        int fd;
        sockaddr_in addr;

        socket(int _fd);
        void checkup(int code) const;
    };
}
