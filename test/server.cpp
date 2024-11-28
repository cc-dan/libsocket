#include <iostream>
#include <string>
#include <socket.h>

int main()
{
    network::socket socket = network::socket();
    socket.open(1234);
    socket.listen(1);
    
    network::socket client = socket.accept();

    while (true)
    {
        std::string data = client.receive(512);
        std::cout << data << std::endl;
    
        if (!data.size())
            break;
    }

    return 0;
}
