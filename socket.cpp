#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <exception>
#include <string.h>
#include "socket.h"

network::socket::socket() 
{
    checkup(fd = ::socket(AF_INET, SOCK_STREAM, 0));
    
    std::cout << "created socket: " << fd << std::endl;
}

network::socket::socket(int _fd) : fd(_fd) 
{
    std::cout << "created child socket: " << fd << std::endl;
}

/*
ife::network::socket&
ife::network::socket::operator=(ife::network::socket &&s)
{
    fd = s.fd;
    addr = s.addr;
    s.fd = network::invalid_socket;
    return *this;
}*/

network::socket::~socket()
{
    close(fd);
    
    std::cout << "closed: " << fd << std::endl;
}

void 
network::socket::open(network::port local)
{   
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(local);
    
    checkup(bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof addr));
    
    std::cout << "opened on port " << local << std::endl;
}

void 
network::socket::listen(unsigned limit) const
{
    checkup(::listen(fd, limit));
    
    std::cout << "listening" << std::endl;
}

network::socket 
network::socket::accept() const
{
    sockaddr_in remote;
    socklen_t size = sizeof(remote);

    int sockfd = ::accept(fd, reinterpret_cast<sockaddr*>(&remote), &size);
    checkup(sockfd);
    
    std::cout << "connection from " << sockfd << ". making socket" << std::endl;
    
    socket s(sockfd);
    s.addr = remote;
    return s;
}

void 
network::socket::connect(const std::string &ip, network::port remote)
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(remote);

    checkup(::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof addr));
    
    std::cout << "connected" << std::endl;
}

void 
network::socket::checkup(int code) const
{
    if (code == -1)
        throw std::runtime_error(strerror(errno));
}

void 
network::socket::send(const char *data, unsigned size) const
{   
    unsigned sent = 0;

    while (size > 0)
    {
        std::cout << "writing" << std::endl;
        
        unsigned bytes;
        checkup(bytes = ::send(fd, data + sent, size, MSG_NOSIGNAL));
        
        if (!bytes) break;
        
        sent += bytes;
        size -= bytes;
    }
    
    std::cout << "wrote: " << data << ((size > 0)? " (missing information)" : "") << std::endl;
}

void
network::socket::send(const std::string &expression) const
{
    send(expression.c_str(), expression.size());
}

int
network::socket::receive(char *buffer, unsigned size) const
{
    std::cout << "reading" << std::endl;
    
    unsigned read;
    checkup(read = ::recv(fd, buffer, size, 0));
    
    std::cout << "read " << read << " bytes" << std::endl;
    
    return read;
}

std::string 
network::socket::receive(unsigned quota) const
{
    char message[quota];
    
    unsigned length = receive(message, quota);
    
    message[length] = 0;
    
    std::cout << "read as string: " << message << std::endl;
    
    return std::string(message);
}

int
network::socket::get_fd() const
{
    return fd;
}

void 
network::socket::shutdown() const
{
    ::shutdown(fd, SHUT_RDWR);
}