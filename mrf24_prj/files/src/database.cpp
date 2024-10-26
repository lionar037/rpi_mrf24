//////////////////////////////////////////////////////////////////////////////
//     
//          filename            :   database.cpp
//          License             :   GNU 
//          Author              :   Lio
//          Change History      :
//          Processor           :   ARM
//          Hardware            :		
//          Compiler            :   ARM
//          Company             :   lionar037
//          Dependencies        :   MySQL Connector/C++
//          Description         :   Implementación de funciones para la gestión 
//                                de datos en una base de datos MySQL.
//          @brief               :   Esta clase proporciona métodos para 
//                                conectarse a una base de datos MySQL y 
//                                recuperar datos específicos usando un ID.
//                                Incluye manejo de excepciones para 
//                                errores de conexión y consulta.
// 
//////////////////////////////////////////////////////////////////////////////

// Instalar la biblioteca MySQL Connector/C++ si no está instalada
//# sudo apt-get install libmysqlcppconn-dev -y

#include <files/include/database.h> // Incluir el archivo de cabecera de la clase Database_t
#include <memory> // Para el uso de std::unique_ptr
#include <iostream> // Para la salida estándar
#include <sstream> // Para el uso de std::ostringstream
#include <iomanip> // Para manipuladores de flujo de formato
#include <string> // Para la clase std::string
#include <mysql_driver.h> // Para el driver de MySQL
#include <mysql_connection.h> // Para la clase Connection
#include <cppconn/driver.h> // Para el driver C++
#include <cppconn/exception.h> // Para excepciones
#include <cppconn/resultset.h> // Para ResultSet
#include <cppconn/statement.h> // Para Statement

namespace DATABASE { // Declaración del espacio de nombres DATABASE

    // Método para recuperar datos de la base de datos según el ID proporcionado
    void Database_t::fetchData(int idToRetrieve) {
        try {
            sql::mysql::MySQL_Driver *driver; // Puntero al driver de MySQL
            sql::Connection *con; // Puntero a la conexión

            // Crear una instancia del driver de MySQL
            driver = sql::mysql::get_mysql_driver_instance();
            // Establecer una conexión con el servidor MySQL
            con = driver->connect(host_, user_, password_);

            // Seleccionar la base de datos
            con->setSchema(database_);

            // Crear una declaración SQL para ejecutar la consulta
            sql::Statement *stmt = con->createStatement();
            // Ejecutar la consulta y almacenar el resultado
            sql::ResultSet *res = stmt->executeQuery("SELECT id, hex_data, NAME, DATA, DATE FROM mrf24Table WHERE id = " + std::to_string(idToRetrieve));

            // Mostrar los resultados si se encuentra un registro
            if (res->next()) {
                // Obtener el ID del registro
                //auto id = res->getInt("id");

                // Obtener un objeto istream para los datos binarios
                std::istream *hexDataStream = res->getBlob("hex_data");

                // Convertir el istream a una representación hexadecimal
                std::ostringstream hexData; // Objeto para construir la cadena hexadecimal
                hexData << std::hex << std::setfill('0'); // Configurar formato hexadecimal
                unsigned char buffer; // Buffer para almacenar los bytes

                // Leer los datos del flujo y convertirlos a hexadecimal
                while (hexDataStream->get(reinterpret_cast<char&>(buffer))) {
                    hexData << std::setw(2) << static_cast<int>(buffer); // Formatear el byte como hexadecimal
                }
                delete hexDataStream; // Liberar el flujo de datos

                // Debug: Imprimir información del registro recuperado
                #ifdef DBG_DATABASE
                    std::cout << "ID: " << id << "\n";
                    std::cout << "Hex Data: " << hexData.str() << "\n";
                    std::cout << "Name: " << res->getString("NAME") << "\n";
                    std::cout << "Data: " << res->getString("DATA") << "\n";
                    std::cout << "Date: " << res->getString("DATE") << "\n";
                #endif
            } else {
                // Mensaje en caso de que no se encuentre un registro con el ID proporcionado
                std::cout << "No se encontró un registro con el ID " << idToRetrieve << "\n";
            }

            // Liberar recursos
            delete res; // Liberar el conjunto de resultados
            delete stmt; // Liberar la declaración
            delete con; // Liberar la conexión
        } catch (sql::SQLException &e) {
            // Manejo de excepciones SQL
            #ifdef DBG_DATABASE
                std::cout << "# ERR: SQLException en " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
                std::cout << "# ERR: " << e.what();
                std::cout << " (Código de error MySQL: " << e.getErrorCode();
                std::cout << ", Estado SQL: " << e.getSQLState() << " )\n";
            #endif
        }
    }

    // Método de inicialización para establecer conexión y recuperar datos
    void Database_t::init() {
        // Definir parámetros de conexión
        const std::string host = "tcp://" + std::string(HOSTNAME_DATABASE) + ":3306"; // URL del host de la base de datos
        const std::string user = "user1"; // Usuario para la conexión
        const std::string password = "passwd"; // Contraseña del usuario
        const std::string database = "databaseMDB"; // Nombre de la base de datos

        // Crear una instancia única de Database_t
        std::unique_ptr<Database_t> databaseInstance{std::make_unique<Database_t>(host, user, password, database)};

        const int idToRetrieve = 64; // ID del registro a recuperar
        databaseInstance->fetchData(idToRetrieve); // Llamar al método para recuperar datos
    }
}
