#include <iostream>
#include <epollcxx.h>

int main()
{
    NetAddress Net("1.2.3.4", 6000);

    std::cout << Net.GetIpAddress() << " " << Net.GetPort() << std::endl;
    
    GEpollService = new EpollService;

    if(false == GEpollService->Initialize(5000, 1000))
        return -1;
    
    

}
