#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Get port input
    int port;
    std::cout << "Select the port number: ";
    std::cin >> port;
    addr.sin_port = htons(port);


    bind(listener, (SOCKADDR*)&addr, sizeof(addr));    
    std::cout << "Server is running on port " << port << "\n";

    // Get file path input
    std::string path;
    std::cout << "Write the destionation path: ";
    std::cin >> path;

    char buf[2048];
    int ret, sum = 0;
    FILE* f = fopen(path.c_str(), "wb");


    while (1) {
        ret = recvfrom(listener, buf, sizeof(buf), 0, NULL, NULL);
        if (ret <= 0)
            break;

        fwrite(buf, 1, ret, f);

        sum += ret;
        printf("\r%d", sum);
    }

    printf("\nFinished");

    fclose(f);

    closesocket(listener);
    WSACleanup();
}