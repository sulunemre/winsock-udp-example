#include <stdio.h>
#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;

    // Get address input
    std::string serverAddress;
    std::cout << "Write the server address: ";
    std::cin >> serverAddress;
    addr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
    
    // Get port input
    int port;
    std::cout << "Select the port number: ";
    std::cin >> port;
    addr.sin_port = htons(port);

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("connect() failed: error code %d\n", ret);
        return 1;
    }

    // Get file path input
    std::string path;
    std::cout << "Write the destionation path: ";
    std::cin >> path;

    FILE* f = fopen(path.c_str(), "rb");
    char buf[2048];

    printf("Sending: ");
    int sum = 0;

    while (1)
    {
        ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0)
            break;

        send(client, buf, ret, 0);

        sum += ret;
        printf("\r%d", sum);
    }

    printf("\nFinished");

    fclose(f);

    closesocket(client);
    WSACleanup();

    return 0;
}