#include <iostream>
#include <windows.h>
#include <string>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <fstream>

#define SQL_RETURN_CODE_LEN 1000
void show_error(unsigned int handletype, const SQLHANDLE& handle) {
    SQLCHAR sqlstate[SQL_RETURN_CODE_LEN];
    SQLCHAR message[SQL_RETURN_CODE_LEN];
    if (SQL_SUCCESS == SQLGetDiagRec(handletype,
        handle,
        1,
        sqlstate,
        NULL,
        message,
        SQL_RETURN_CODE_LEN,
        NULL)) {
        std::cout << "Message: " << message << "\nSQLSTATE: " << sqlstate << std::endl;
    }
}

bool Сonnect_DBMS(SQLHANDLE& envHandle, SQLHANDLE& connHandle, SQLRETURN& ret) {

    SQLCHAR* connStr = (SQLCHAR*)"DRIVER={SQL Server};SERVER=LAPTOP-MG3M03B5\\SQLEXPRESS;Trusted_Connection=Yes;";
    ret = SQLDriverConnect(connHandle, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);//установление соединения с базой данных

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Подключение к SSMS успешно установлено." << std::endl;
        return true;
    }
    return false;
}

bool Create_DB(SQLHANDLE& stmtHandle, SQLRETURN& ret, std::string Name_db, std::string filename) {
    std::string query = "CREATE DATABASE " + Name_db + "; ";
    SQLRETURN answer = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }

    std::cout << "База данных " + Name_db + " успешно создана." << std::endl;
    query.clear();

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка при открытии файла" << std::endl;
        return 1;
    }
    std::string line;

    // Читаем содержимое файла построчно и сохраняем в строку query
    while (std::getline(file, line)) {
        query += line;
        query += '\n';
    }

    SQLCHAR* create_tables = (SQLCHAR*)query.c_str();
    if (0 >= SQLExecDirect(stmtHandle, create_tables, SQL_NTS)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    return true;

    ret = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Создание сущностей произошло успешно." << std::endl;
        return true;
    }
    else {
        std::cout << "Не удалось создать сущности!" << std::endl;
        return false;
    }

}

bool Join_db(const std::string type_car, SQLHANDLE& connHandle, SQLHANDLE& stmtHandle) {
    std::string query = "INSERT INTO db_Result.." + type_car + " SELECT * FROM db_Norm.." + type_car;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    SQLRETURN answer = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    query = "INSERT INTO db_Result.." + type_car + " SELECT * FROM db3_mirror.." + type_car;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    answer = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    return true;
}

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    //// Выполнить инициализацию ODBC
    SQLHANDLE envHandle;//для хранения дескриптора среды ODBC
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);//Идентификатор окружения. Выделяется память и инициализируется дескриптор среды ODBC.
    SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_POINTER);//Устанавливается атрибут среды ODBC

    // Создать и открыть подключение к SSMS
    SQLHANDLE connHandle;//будет использоваться для хранения дескриптора подключения к базе данных.
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle);//Идентификатор соединения. Выделяется память и инициализируется дескриптор подключения к базе данных 
    SQLRETURN ret;

    if (!Сonnect_DBMS(envHandle, connHandle, ret)) {
        std::cout << "Не удалось установить подключение к SSMS." << std::endl;
        return 1;
    }

    SQLHANDLE stmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса

    //Создать базу данных
    if (!Create_DB(stmtHandle, ret, "db_Result", "C:\\Graduate_work\\Creating_Result.sql")) {
        return 1;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    if (!Join_db("BUS", connHandle, stmtHandle)) {
        std::cout << "Ошибка при объединении таблиц с типом ТС \"BUS\"";
        return 1;
    }
    if (!Join_db("HCV", connHandle, stmtHandle)) {
        std::cout << "Ошибка при объединении таблиц с типом ТС \"HCV\"";
        return 1;
    }
    if (!Join_db("LCV", connHandle, stmtHandle)) {
        std::cout << "Ошибка при объединении таблиц с типом ТС \"LCV\"";
        return 1;
    }
    if (!Join_db("MT", connHandle, stmtHandle)) {
        std::cout << "Ошибка при объединении таблиц с типом ТС \"MT\"";
        return 1;
    }
    if (!Join_db("PC", connHandle, stmtHandle)) {
        std::cout << "Ошибка при объединении таблиц с типом ТС \"PC\"";
        return 1;
    }
    
}