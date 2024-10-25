//# sudo apt-get install libmysqlcppconn-dev -y

#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


#include <files/include/database.h>

namespace DATABASE{

    void Database_t::fetchData(int idToRetrieve) {
        try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // Crear una conexión
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host_, user_, password_);

        // Seleccionar la base de datos
        con->setSchema(database_);

        // Recuperar el valor del ID
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT id, hex_data, NAME, DATA, DATE FROM mrf24Table WHERE id = " + std::to_string(idToRetrieve));

        // Mostrar resultados
        if (res->next()) {
            //auto id = 
            res->getInt("id");

            // Obtener un objeto istream para los datos binarios
            std::istream *hexDataStream = res->getBlob("hex_data");

            // Convertir el istream a una representación hexadecimal
            std::ostringstream hexData;
            hexData << std::hex << std::setfill('0');
            unsigned char buffer;
            while (hexDataStream->get(reinterpret_cast<char&>(buffer))) {
                hexData << std::setw(2) << static_cast<int>(buffer);
            }
            delete hexDataStream;
            #ifdef DBG_DATABASE
                std::cout << "ID: " << id << "\n";
                std::cout << "Hex Data: " << hexData.str() << "\n";
                std::cout << "Name: " << res->getString("NAME") << "\n";
                std::cout << "Data: " << res->getString("DATA") << "\n";
                std::cout << "Date: " << res->getString("DATE") << "\n";
            #endif
        } else {
            std::cout << "No se encontró un registro con el ID " << idToRetrieve << "\n";
        }

        delete res;
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        #ifdef DBG_DATABASE
            std::cout << "# ERR: SQLException en " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
            std::cout << "# ERR: " << e.what();
            std::cout << " (Código de error MySQL: " << e.getErrorCode();
            std::cout << ", Estado SQL: " << e.getSQLState() << " )\n";
        #endif
    }
}

    void Database_t::init() {
    std::string host = "tcp://" +  std::string(HOSTNAME_DATABASE) + ":3306";
    std::string user = "user1";
    std::string password = "passwd";
    std::string database = "databaseMDB";

    std::unique_ptr<Database_t> databaseInstance{std::make_unique<Database_t>(host, user, password, database)};

    int idToRetrieve = 64;
    databaseInstance->fetchData(idToRetrieve);
    
    }
}































































