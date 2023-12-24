#include <iostream>
//#include <thread>
#include <memory>
#include <unistd.h>
#include <others/src/rfflush.h>
#include <others/src/color.h>
#include <app/src/config.h>
#include <others/src/msj.h>

namespace FFLUSH{

static DEVICES::Msj_t msj;

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
        
        std::string dat = "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H" + str_txt.data();  
        msj.set(dat.data());
        //std::cout << "\033[" << row << ";" << col << "H"<<str_txt.data();    
        
        return ;
    }
    void Fflush_t::maxLines(int x){
        msj.setMaxLines(x);
    }

    void Fflush_t::view(){
        #ifdef ENABLE_PRINTS_DBG  
        msj.printData();
        #endif
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


