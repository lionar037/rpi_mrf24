#include <iostream>
#include <iomanip>
#include <sstream>

#include <openssl/evp.h>
#include <openssl/sha.h>  // Incluye SHA256_DIGEST_LENGTH

#include <cstring>
#include <termios.h>
#include <security/include/security.h> // Asegúrate de incluir el encabezado correcto

namespace SECURITY {

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

    // Función para calcular el hash SHA-256 usando la API EVP
    std::string Security_t::sha256(const std::string& str) 
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];  // Asegúrate de que SHA256_DIGEST_LENGTH esté disponible
        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();  // Crear contexto para el hash

        if (mdctx == nullptr) {
            throw std::runtime_error("Error creating EVP_MD_CTX");
        }

        if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Error initializing EVP for SHA256");
        }

        if (EVP_DigestUpdate(mdctx, str.c_str(), str.size()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Error updating SHA256 hash");
        }

        unsigned int hash_len;
        if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Error finalizing SHA256 hash");
        }

        EVP_MD_CTX_free(mdctx);  // Liberar el contexto

        // Convertir el hash a una cadena hexadecimal
        std::stringstream ss;
        for (unsigned int i = 0; i < hash_len; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        return ss.str();
    }

    int Security_t::init() {
        std::cout << "\nUser : "<< USER <<"\n" << "\tPassword: ";
        m_inputPassword = getHiddenInput();

        auto result = sha256(PASSWORD_SAVE) == sha256(m_inputPassword);
        if (result == 1) {
            std::cout << "\n\t\tsuccess\n";
            return SUCCESS_PASS;
        }
        std::cout << "\nEl password no es válido\n";
        std::cout << "No se pudo enviar el mensaje \n";
        return -1;
    }

    std::string Security_t::encrypt(std::string_view& encTmp) {
        return sha256(encTmp.data()); 
    }

}
