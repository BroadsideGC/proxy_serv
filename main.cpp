
#include <iostream>

#include "proxy_server.h"

using namespace std;


int main() {
    // freopen("debug.txt", "w", stderr);
    proxy_server server(8080);
    server.run();
    return 0;

}