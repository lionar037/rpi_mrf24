#pragma once
#include <string>

#define ID_SEARCH 64
#define HOSTNAME_DATABASE "192.168.1.38"

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