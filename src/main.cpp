/*
author : lion
*/


//#include <run.h>
//#include <memory>
//#include <mrf24/src/radio.h>


#include <memory>
#include <tuple>
#include <vector>
#include <string_view>
#include <iostream>






int main(){
    auto qr = std::make_unique<QrOled_t>();
 
    // std::string_view packet_data = "ljwekjnwldnlwwnx";
    // std::vector<int> bf_tmp; 
    // qr->create_qr(packet_data, bf_tmp);
// 
    // std::cout << "Size of bf_tmp: " << bf_tmp.size() << std::endl;

    return 0;
}
