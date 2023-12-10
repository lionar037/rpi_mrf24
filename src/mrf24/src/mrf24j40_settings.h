#pragma once
#include <iostream>

namespace MRF24J40{
    typedef struct rxmcr{
        uint8_t PROMI       :1; //1 = Receive all packet types with good CRC
                                //0 = Discard packet when there is a MAC address mismatch, illegal frame type, dPAN/sPAN or MAC //short address mismatch (default)
        uint8_t ERRPKT      :1;
        uint8_t COORD       :1; //1 = Set device as coordinator
                                //0 = Device is not print as coordinator (default)

        uint8_t PANCOORD    :1; //1 = Set device as PAN coordinator
                                //0 = Device is not print as PAN coordinator (default)
        uint8_t Reserved_0      :1;
        uint8_t NOACKRSP        :1;
        uint8_t Reserved_1      :2;

    }RXMCR;
}

    // typedef struct rxmcr{
    //    uint8_t PROMI :1; //1 = Receive all packet types with good CRC
    //                            //0 = Discard packet when there is a MAC address mismatch, illegal frame type, dPAN/sPAN or MAC //short address mismatch (default)
    //    uint8_t ERRPKT  :1;
    //    uint8_t COORD   :1; //1 = Set device as coordinator
    //                            //0 = Device is not print as coordinator (default)
    //    uint8_t PANCOORD :1; //1 = Set device as PAN coordinator
    //                            //0 = Device is not print as PAN coordinator (default)
    //    uint8_t Reserved_0  :1;
    //    uint8_t NOACKRSP    :1;
    //    uint8_t Reserved_1  :1;
    //    uint8_t Reserved_2  :1;
    //}RXMCR;