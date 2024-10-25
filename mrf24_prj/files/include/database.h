#pragma once
#include <string>

#define ID_SEARCH 64


#if defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4)
    // Si es de 32 bits
    #define HOSTNAME_DATABASE "raspberrypi.local"     
#else
    // Si es de 64 bits
    #define HOSTNAME_DATABASE "localhost"
    //#define HOSTNAME_DATABASE "mosquitto.local"
#endif

//#define HOSTNAME_DATABASE "192.168.1.38"
//#define HOSTNAME_DATABASE "raspberry.local"
//#define HOSTNAME_DATABASE "rpi2w.local"



namespace DATABASE{


struct Database_t {
        Database_t(){
                init();
            }
            
        Database_t(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
            : host_(host), user_(user), password_(password), database_(database) {
               
            }

        void fetchData(int idToRetrieve) ;    
        void init();

    private:
        std::string host_;
        std::string user_;
        std::string password_;
        std::string database_;
    };

}