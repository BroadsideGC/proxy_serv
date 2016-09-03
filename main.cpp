
#include <iostream>
#include <thread>

#include "proxy_server.h"

using namespace std;



int main() {
   // freopen("/home/bigz/proxy/debug.txt", "w", stdout);
    std::cout<<"Port: 8080";
   proxy_server server(8080);
    server.run();
    return 0;

}