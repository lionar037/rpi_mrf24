

#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cstring>
#include <termios.h>
#include <security/include/security.h>


namespace SECURITY{

// Función para ocultar la entrada del usuario
  std::string Security_t::getHiddenInput() 
  {
      std::string input;
      struct termios oldt, newt;

      // Guardar configuración actual del terminal
      tcgetattr(0, &oldt); // Puedes usar 0 directamente en lugar de STDIN_FILENO
      newt = oldt;

      // Desactivar la impresión en la pantalla
      newt.c_lflag &= ~ECHO;

      // Aplicar la nueva configuración
      tcsetattr(0, TCSANOW, &newt);

      // Leer la entrada del usuario
      std::getline(std::cin, input);

      // Restaurar la configuración original del terminal
      tcsetattr(0, TCSANOW, &oldt);

      return input;
  }

  // Función para calcular el hash SHA-256
  std::string Security_t::sha256(const std::string& str) 
  {
      unsigned char hash[SHA256_DIGEST_LENGTH];
      SHA256_CTX sha256;
[[gnu::unused]] SHA256_Init(&sha256);
      SHA256_Update(&sha256, str.c_str(), str.size());
      SHA256_Final(hash, &sha256);

      std::stringstream ss;
      for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
          ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
      }
      return ss.str();
  }


  int Security_t::init() {
      
      std::cout << "\nUser : "<< USER <<"\n" << "\tPassword: ";
      m_inputPassword = getHiddenInput();

      auto result = sha256(PASSWORD_SAVE) == sha256(m_inputPassword);
      if (result==1){
        std::cout << "\n\t\tsuccess\n";
        return SUCCESS_PASS;
      }
        std::cout << "\n el password no es valido\n";
        std::cout << "No se pudo enviar el mensaje \n";
      return -1;
  }

    std::string Security_t::encrypt(std::string_view& encTmp){
    return sha256(encTmp.data()); 
    }

}
