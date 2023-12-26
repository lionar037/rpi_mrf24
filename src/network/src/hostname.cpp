#include <iostream>
#include <cstring>
#include <algorithm>
#include <cctype>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <network/src/hostname.h>


namespace NETWORK{
    void Hostname_t::print() {
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        std::cout << "Hostname: " << hostname << std::endl;

        struct ifaddrs *ifAddrStruct = nullptr;
        struct ifaddrs *ifa = nullptr;
        void *tmpAddrPtr = nullptr;

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET) { // IPv4
                tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

                std::cout << "Interface: " << ifa->ifa_name << "\tAddress: " << addressBuffer << std::endl;
            }
        }

        if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);

        return ;
    }




    void Hostname_t::GetHostname(std::string& hname) {
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        hname=hostname;
        std::cout << "Hostname: " << hostname << std::endl;

        struct ifaddrs *ifAddrStruct = nullptr;
        struct ifaddrs *ifa = nullptr;
        void *tmpAddrPtr = nullptr;

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET) { // IPv4
                tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
    hname += addressBuffer;

        std::transform(hname.begin(), hname.end(), hname.begin(), ::toupper);

                std::cout << "Interface: " << ifa->ifa_name << "\tAddress: " << addressBuffer << std::endl;
            }
        }

        if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);

        return ;
    }

}