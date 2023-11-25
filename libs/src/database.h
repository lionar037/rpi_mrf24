#pragma once
#include <memory>

namespace DATABASE{


    struct Database_t{
        Database_t();
        ~Database_t();
        bool database() ;
        private:
        std::unique_ptr<sql::mysql::MySQL_Driver> driver{};
        std::unique_ptr<sql::Connection> con{};
    };
}