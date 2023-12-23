#include <iostream>
//#include <thread>
#include <memory>
#include <unistd.h>
#include <others/src/rfflush.h>
#include <others/src/color.h>
#include <app/src/config.h>

namespace FFLUSH{

     void updateValue(int id, int delay, int row, int col) 
     { //row fila  // col : columna 
        // auto ff{std::make_unique<Fflush_t>() };
        // int valor = 0;
        // while (valor <= 100) {
            // std::unique_lock<std::mutex> lock(ff->m_mtx);
        //Mover el cursor a la ubicación de las coordenadas (row, col) y actualizar el valor
            // std::cout << "\033[" << row << ";" << col << "HVALOR " << id << " : " << valor << std::flush;
            // lock.unlock();
            // valor++;
            // usleep(delay);
            // 
        // }
     }

    void Fflush_t::print(std::string_view str_txt, int row, int col) 
    { 
        #ifdef ENABLE_PRINTS_DBG
        //row fila  // col : columna         
     //SET_COLOR(SET_COLOR_GRAY_TEXT);
    // move(row,col);
    std::cout << "\033[" << row << ";" << col << "H"<<str_txt.data();
            //std::unique_lock<std::mutex> lock(m_mtx);
            // Mover el cursor a la ubicación de las coordenadas (row, col) y actualizar el valor
            //std::cout << "\033[" << row << ";" << col << "H" << str_txt.data();//<< std::flush;
            //lock.unlock();                
            //std::cout<<str_txt.data();
            #endif
    return ;    
    }


void Fflush_t::sendMsj(std::string_view str_txt, int row, int col) {


}


    int Fflush_t::funcThread() 
    {
        // Limpia la pantalla y muestra "VALOR 1" y "VALOR 2" en ubicaciones específicas
       // std::cout << "\033[2J\033[HVALOR 1 :\nVALOR 2 :" << std::flush;
    
    //std::cout << "\033[2J\033[H" << std::flush;
    
        // Inicia los hilos para actualizar los valores
        // std::thread thread1(updateValue, 1, 1000000, 2, 9);  // Fila 2, Columna 9
        // std::thread thread2(updateValue, 2, 2000000, 3, 9);  // Fila 3, Columna 9
    // 
        // thread1.join();
        // thread2.join();
    // 
        // std::cout << std::endl;
        return 0;
    }


    
}


