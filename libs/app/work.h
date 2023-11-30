#pragma once
//#include 

namespace FILESYSTEM{
        struct File_t();
    }

namespace WORK{
    struct Work_t: public FILESYSTEM::File_t{
            virtual bool    create(const char*)=0;       
            virtual         ~Work_t()=default;
        private:
    };
}