#pragma once


namespace WORK{

    struct Work_t{
            virtual bool    create(const char*)=0;       
            virtual         ~Work_t()=default;
        private:
    };
}