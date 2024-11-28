#include <iostream>
#include <socket.h>

int main()
{
    network::socket server = network::socket();
    server.connect("127.0.0.1", 1234);
    
    while (true)
    {
        std::string data;
        std::cin >> data;
        
        server.send(data);
    }

    return 0;
}
