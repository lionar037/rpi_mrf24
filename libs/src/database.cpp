//# sudo apt-get install libmysqlcppconn-dev -y

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <sstream>
#include <iomanip>
#include <database.h>

namespace DATABASE{
    
    bool Database_t::database() 
    {
    try {
        //sql::mysql::MySQL_Driver *driver;
        //sql::Connection *con;

        // Crear una conexión
       // driver = sql::mysql::get_mysql_driver_instance();
       
       // con = driver->connect("tcp://127.0.0.1:3306", "admin", "passwd");
       // con = driver->connect("tcp://192.168.1.45:3306", "user1", "passwd");        
        // Seleccionar la base de datos
        con->setSchema("databaseMDB");
        // Recuperar el valor del ID 64
        //int idToRetrieve = ID_SEARCH;
       // sql::Statement *stmt    = con->createStatement();
       // sql::ResultSet *res     = stmt->executeQuery("SELECT id, hex_data, NAME, DATA, DATE FROM mrf24Table WHERE id = " + std::to_string(idToRetrieve));
        // Mostrar resultados
        if (res->next()) {
            int& id = res->getInt("id");
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
            std::cout << "ID: " << id << "\n";
            std::cout << "Hex Data: " << hexData.str() << "\n";
            std::cout << "Name: " << res->getString("NAME") << "\n";
            std::cout << "Data: " << res->getString("DATA") << "\n";
            std::cout << "Date: " << res->getString("DATE") << "\n";
        } else {
            std::cout << "No se encontró un registro con el ID " << idToRetrieve << "\n";
        }
        //delete res;
        //delete stmt;
        //delete con;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException en " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        std::cout << "# ERR: " << e.what();
        std::cout << " (Código de error MySQL: " << e.getErrorCode();
        std::cout << ", Estado SQL: " << e.getSQLState() << " )\n";
    }
    return true;
}


Database_t::Database_t()
://con{std::make_unique<sql::Connection>()} , //driver{std::make_unique<sql::mysql::MySQL_Driver>()}
    idToRetrieve    { ID_SEARCH }
,   driver          { sql::mysql::get_mysql_driver_instance() }
,   con             { driver->connect("tcp://192.168.1.45:3306", "user1", "passwd") }
 ,  stmt            { con->createStatement() }
 ,  res             { stmt->executeQuery("SELECT id, hex_data, NAME, DATA, DATE FROM mrf24Table WHERE id = " + std::to_string(idToRetrieve)) }
 {
  std::cout<<"Database_t()\n";  
}

Database_t::~Database_t(){
  std::cout<<"~Database_t()\n";  
}


}
