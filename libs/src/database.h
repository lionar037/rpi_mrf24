#pragma once
#include <memory>
#define ID_SEARCH 64
namespace DATABASE{


    struct Database_t{
        Database_t();
        ~Database_t();
        bool database() ;
        private:
        const int idToRetrieve {0}; //ID_SEARCH;  
        std::unique_ptr <sql::mysql::MySQL_Driver>  driver  {};
        std::unique_ptr <sql::Connection>           con     {};
        std::unique_ptr <sql::Statement>            stmt    {};    
        std::unique_ptr <sql::ResultSet>            res     {};    
       
    };
}