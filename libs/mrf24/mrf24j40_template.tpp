#pragma once

#include <mrf24/mrf24j40.h>
#include <app/config.h>

namespace MRF24J40
{

    template <typename T>
    void Mrf24j::send(uint64_t dest64, const T& data) 
    {
        const uint8_t len = data.length();//strlen(data); // get the length of the char* array
        int i = 0;
        write_long(i++, m_bytes_MHR); // header length
                        // +ignoreBytes is because some module seems to ignore 2 bytes after the header?!.
                        // default: ignoreBytes = 0;
        write_long(i++, m_bytes_MHR+ignoreBytes+len);

                        // 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
        write_long(i++, 0b01100001); // first byte of Frame Control
                        // 16 bit source, 802.15.4 (2003), 16 bit dest,
        write_long(i++, 0b10001000); // second byte of frame control
        write_long(i++, 1);  // sequence number 1

        const uint16_t panid = get_pan();
        #ifdef DBG
            printf("\npanid: 0x%X\n",panid);
        #endif
        write_long(i++, panid & 0xff);  // dest panid
        write_long(i++, panid >> 8);

        write_long(i++, dest64  & 0xff); // uint64_t
        write_long(i++, (dest64 >> 8  ) & 0xff);
        write_long(i++, (dest64 >> 16 ) & 0xff);
        write_long(i++, (dest64 >> 24 ) & 0xff);
        write_long(i++, (dest64 >> 32 ) & 0xff);
        write_long(i++, (dest64 >> 40 ) & 0xff);
        write_long(i++, (dest64 >> 48 ) & 0xff);
        write_long(i++, (dest64 >> 56 ) & 0xff);

        const uint64_t src64 = address64_read();
        write_long(i++, src64  & 0xff ); // uint64_t
        write_long(i++, (src64 >> 8  ) & 0xff); 
        write_long(i++, (src64 >> 16 ) & 0xff); 
        write_long(i++, (src64 >> 24 ) & 0xff); 
        write_long(i++, (src64 >> 32 ) & 0xff); 
        write_long(i++, (src64 >> 40 ) & 0xff); 
        write_long(i++, (src64 >> 48 ) & 0xff); 
        write_long(i++, (src64 >> 56 ) & 0xff); 

                // All testing seems to indicate that the next two bytes are ignored.
                //2 bytes on FCS appended by TXMAC
        i+=ignoreBytes;

        //for(const auto& byte : data) write_long(i++,static_cast<char>(byte));
        if(data.head==HEAD)write_long(i++,data.head);
        // for(const auto& byte : static_cast<const char *>(buf.size) )
        write_long(i++,data.size&0xff);
        write_long(i++,(data.size>>8)&0xff);

        for(const auto& byte : data.data )write_long(i++,byte);

        // ack on, and go!
        write_short(MRF_TXNCON, (1<<MRF_TXNACKREQ | 1<<MRF_TXNTRIG));
    }


}
