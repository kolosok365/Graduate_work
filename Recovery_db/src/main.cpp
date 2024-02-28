#include <iostream>
#include <windows.h>
#include <string>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <vector>
#include <tuple>
#include <any>
#include <variant>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <sstream>

SQLHANDLE envHandle;//для хранения дескриптора среды ODBC
SQLHANDLE connHandle1;//будет использоваться для хранения дескриптора подключения к базе данных.
SQLHANDLE connHandle2;
SQLHANDLE connHandle3;

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

bool Create_DB(SQLHANDLE& stmtHandle, SQLRETURN& ret, std::string Name_db) {
    std::string query = "CREATE DATABASE " + Name_db + "; ";
    SQLRETURN answer = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!(answer == SQL_SUCCESS || answer == SQL_SUCCESS_WITH_INFO)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }

    std::cout << "База данных " + Name_db + " успешно создана." << std::endl;
    query = "USE db3\n SELECT * INTO db3..PC FROM db_Gaps..PC Where 1 = 2\n"
            "SELECT * INTO db3..LCV FROM db_Gaps..LCV Where 1 = 2\n"
            "SELECT * INTO db3..HCV FROM db_Gaps..HCV Where 1 = 2\n"
            "SELECT * INTO db3..MT FROM db_Gaps..MT Where 1 = 2\n"
            "SELECT * INTO db3..TR  FROM db_Gaps..TR Where 1 = 2\n"
            "SELECT * INTO db3..BUS FROM db_Gaps..BUS Where 1 = 2";

    SQLCHAR* create_tables = (SQLCHAR*)query.c_str();
    if (0 >= SQLExecDirect(stmtHandle, create_tables, SQL_NTS)) {
        show_error(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    return true;
}

void Insert_row(const std::string& type_car, std::unordered_map<std::string, std::string>&& sample_rows) {
    SQLHANDLE stH_one_row;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle3, &stH_one_row);
    std::string insert_query = "INSERT into db3_mirror.dbo." + type_car;
    std::string cols = "(", values = "(";
    for (auto& v : sample_rows) {
        cols += v.first + ",";
        values += v.second + ",";
    }
    cols.pop_back();//для удаления последней запятой
    values.pop_back();
    insert_query += cols + ") VALUES" + values + ")";
    auto a = SQLExecDirect(stH_one_row, (SQLCHAR*)insert_query.c_str(), SQL_NTS);
    if (!(a == SQL_SUCCESS || a == SQL_SUCCESS_WITH_INFO)) {
        std::cout << "Ошибка вставки кортежа в db3_mirror!";
        show_error(SQL_HANDLE_STMT, stH_one_row);
        system("pause");
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stH_one_row);
    std::cout << "";
}

bool Getting_a_sample(const std::string& query2, std::vector<std::string>& col_names,const std::string& type_car, int kol_in_db_Gaps_row) {
    SQLCHAR columnValue[256];
    SQLLEN cbColumnValue;
    SQLSMALLINT numCols, numRows;
    double sum_of_quantity = 0;
    SQLHANDLE stH_count;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle2, &stH_count);
    auto an = SQLExecDirect(stH_count, (SQLCHAR*)query2.c_str(), SQL_NTS);

    if (an != SQL_SUCCESS) {
        std::cout << "stop";
        return false;
    }

    auto answer_query2 = SQLFetch(stH_count);
    SQLNumResultCols(stH_count, &numCols);
    while (answer_query2 == SQL_SUCCESS) {//цикл для подсчёта суммы всех авто в выборке
        for (int i = 0; i < numCols; ++i) {
            SQLGetData(stH_count, (i + 1), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString(reinterpret_cast<char*>(columnValue));
            if (col_names[i] == "Количество") {
                sum_of_quantity += std::stod(columnValueString);
            }
        }
        answer_query2 = SQLFetch(stH_count);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stH_count);
    if (sum_of_quantity == 0) {
        //std::cout << "Количество строк в выборке = 0";
        return false;
    }
    SQLHANDLE stH;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle2, &stH);
    auto a = SQLExecDirect(stH, (SQLCHAR*)query2.c_str(), SQL_NTS);  
    std::unordered_map<std::string, std::string> sample_rows;
    std::vector<std::unordered_map<std::string, std::string>> vector_sr;
    auto answer_query = SQLFetch(stH);
    while (answer_query == SQL_SUCCESS) {
        for (int i = 0; i < numCols; ++i) {
            SQLGetData(stH, (i + 1), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString(reinterpret_cast<char*>(columnValue));
            if (col_names[i] == "Количество") {
                double probability_q = stod(columnValueString)/sum_of_quantity;
                probability_q *= kol_in_db_Gaps_row;
                sample_rows[col_names[i]] = std::to_string(probability_q);
                std::replace(sample_rows[col_names[i]].begin(), sample_rows[col_names[i]].end(), ',', '.');
                while (sample_rows[col_names[i]].back() == '0') {//для удаления нулей в дробной части
                    sample_rows[col_names[i]].pop_back();
                }
                sample_rows[col_names[i]] = "\'" + sample_rows[col_names[i]] + "\'";
            }
            else if (col_names[i] == "Срез_парка" || columnValueString == "Старше 1987" || columnValueString == "Нет данных") {
                sample_rows[col_names[i]] = "\'" + columnValueString + "\'";
            }
            else {
                sample_rows[col_names[i]] = columnValueString;
            }
        }       
        Insert_row(type_car, std::move(sample_rows));
        answer_query = SQLFetch(stH);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stH);
    return true;
}

void Delete_substr(std::string& str, std::string sub) {
    auto it = str.find(sub);
    int i = 0;
    for (i = it; str[i] != 'A' && str[i] != 'O' /*&& str[i] != '\n'*/; ++i) {}
    if (str[i] == 'A') {
        str.erase(it, ((i - it) + 3));//для удаления подстроки и AND
    }
    else {
        str.erase((it-4), (i - it + 4));////для удаления AND и подстроки до ORDER BY
    }
    
}

void Delete_row(const std::string& query, const std::string& type_car, std::string& id) {
    SQLHANDLE stmtHandle2;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle2, &stmtHandle2);//идентификатор запроса
    std::string delete_query = query + type_car + " where " + "ID_" + type_car + " = " + id;
    auto a = SQLExecDirect(stmtHandle2, (SQLCHAR*)delete_query.c_str(), SQL_NTS);
    if (a != SQL_SUCCESS) {
        std::cout << "Ошибка удаления кортежа!";
        system("pause");
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle2);
}

bool Sample_reduction_by_ONE_field(std::string& copy_query2, std::string ID_Type, std::vector<std::string>& col_names,
                                   const std::string& type_car, std::vector<std::string>& unknown_cols, const std::string& field, 
                                   SQLHANDLE& stmtHandle, int kol_in_db_Gaps_row) {//попытка сократить выборку
    if (std::find(unknown_cols.begin(), unknown_cols.end(), field) == unknown_cols.end()) {
        Delete_substr(copy_query2, field);
        if (Getting_a_sample(copy_query2, col_names, type_car, kol_in_db_Gaps_row)) {
            Delete_row("DELETE FROM db_Gaps_mirror.dbo.", type_car, ID_Type);
            return true;
        };
    }
    return false;
}

bool Sample_reduction_by_TWO_field(std::string& copy_query2, std::string ID_Type, std::vector<std::string>& col_names,
    const std::string& type_car, std::vector<std::string>& unknown_cols, const std::string& field1, const std::string& field2,
    SQLHANDLE& stmtHandle, int kol_in_db_Gaps_row) {//попытка сократить выборку
    if ((std::find(unknown_cols.begin(), unknown_cols.end(), field1) == unknown_cols.end())
        && std::find(unknown_cols.begin(), unknown_cols.end(), field2) == unknown_cols.end()) {
        Delete_substr(copy_query2, field1);
        Delete_substr(copy_query2, field2);
        if (Getting_a_sample(copy_query2, col_names, type_car, kol_in_db_Gaps_row)) {
            Delete_row("DELETE FROM db_Gaps_mirror.dbo.", type_car, ID_Type);
            return true;
        };
    }
    return false;
}

bool Get_records_with_identical_columns(const std::string& query, const std::string& type_car, std::vector<std::string>&& col_names, 
                                        SQLHANDLE& stmtHandle, SQLHANDLE& envHandle, SQLHANDLE& connHandle)
{
    auto a1 = SQLExecDirect(stmtHandle, (SQLCHAR*)(query+type_car).c_str(), SQL_NTS);
    SQLLEN cbColumnValue;
    SQLSMALLINT numCols;
    SQLNumResultCols(stmtHandle, &numCols);
    auto answer_query = SQLFetch(stmtHandle);
    if (answer_query == SQL_NO_DATA) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    std::vector<std::pair<std::string, std::string>> fields;
    std::string query2 = "";
    std::vector<std::string> unknown_cols;
    std::string ID_Type;

    int kol_in_db_Gaps_row;
    // Чтение строк результата запроса
    while (answer_query == SQL_SUCCESS) {
        SQLCHAR columnValue[256];
        ID_Type.clear();
        fields.clear();
        unknown_cols.clear();
        
        for (int i = 0; i < numCols; ++i) {
            SQLGetData(stmtHandle, (i+1), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString(reinterpret_cast<char*>(columnValue));
            fields.push_back({ col_names[i],columnValueString });
            if (col_names[i] == "Количество") {
                kol_in_db_Gaps_row = stoi(columnValueString);
            }
        }

        query2 = "select * from db_Norm.dbo." + type_car + " where ";
        for (auto& [col_name, cell] : fields) {
            if (col_name == "ID_марки" || col_name == "ID_модели" || col_name == "ID_типа_кузова" || col_name == "ID_типа_топлива" ||
                col_name == "Объём_двигателя" || col_name == "Мощность" || col_name == "Экологический_класс" || 
                /*col_name == "ID_места_производства" || col_name == "ID_страны_производства" ||*/ col_name == "ID_типа_сегмента_автостата" || 
                /*col_name == "ID_расположения_руля" ||*/ col_name == "Полная_масса" || col_name == "Снаряжённая_масса" || 
                col_name == "ID_вида_кузова" || col_name == "ID_типа_прицепа" || col_name == "ID_типа_кузова_прицепа" || 
                col_name == "Количество_осей_прицепа" || col_name == "Количество_шин_прицепа" || col_name == "Год_выпуска")
            {
                if (cell != "NULL") {
                    //std::vector<std::string>::iterator itr = std::find(col_names.begin(), col_names.end(), col_name);
                    if (col_name == "ID_типа_топлива") {
                        query2 += "(" + col_name + "=" + cell + " OR " + col_name + " = 1)" + " AND ";
                    }
                    else if (col_name == "Полная_масса" || col_name == "Снаряжённая_масса") {
                        query2 += "(" + col_name + "=\'" + cell + "\' OR " + col_name + " = \'Нет данных\')" + " AND ";
                    }
                    else if (col_name == "Год_выпуска" || col_name == "Экологический_класс" || col_name == "Мощность" || col_name == "Объём_двигателя") {
                        query2 += col_name + "=\'" + cell + "\' AND ";
                    }
                    else {
                        query2 += col_name + "=" + cell + " AND ";
                    }
                }
                else {
                    unknown_cols.push_back(col_name);
                }              
            }
            else if (col_name == ("ID_"+type_car)) {//тут и остальные типы наверное придется ловить
                ID_Type = cell;
            }
        }

        for(int j = 0; j<4;++j)
            query2.pop_back();
        query2 += "\nORDER BY ";
        for (auto& var : unknown_cols) {
            query2 += var + ",";
        }
        query2.pop_back();
        
        std::string copy_query2 = query2;
        if (Getting_a_sample(copy_query2, col_names, type_car, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            Delete_row("DELETE FROM db_Gaps_mirror.dbo.", type_car, ID_Type);
            continue;
        };
        copy_query2 = query2;
        auto end = col_names.end();
        bool has_field = std::count(col_names.begin(), col_names.end(), "Год_выпуска");//для проверки содержится ли такой столбец в таблице
        if (has_field && Sample_reduction_by_ONE_field(copy_query2, ID_Type, col_names, type_car, unknown_cols, "Год_выпуска", stmtHandle, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            continue;
        }
        copy_query2 = query2;
        has_field = std::count(col_names.begin(), col_names.end(), "Объём_двигателя");
        if (has_field && Sample_reduction_by_ONE_field(copy_query2, ID_Type, col_names, type_car, unknown_cols, "Объём_двигателя", stmtHandle, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            continue;
        }
        copy_query2 = query2;
        has_field = std::count(col_names.begin(), col_names.end(), "Мощность");
        if (has_field && Sample_reduction_by_ONE_field(copy_query2, ID_Type, col_names, type_car, unknown_cols, "Мощность", stmtHandle, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            continue;
        }
        copy_query2 = query2;
        has_field = std::count(col_names.begin(), col_names.end(), "Объём_двигателя");
        bool has_field2 = std::count(col_names.begin(), col_names.end(), "Год_выпуска");
        if (has_field && has_field2 && Sample_reduction_by_TWO_field(copy_query2, ID_Type, col_names, type_car, unknown_cols, "Объём_двигателя", "Год_выпуска", stmtHandle, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            continue;
        }
        copy_query2 = query2;
        has_field = std::count(col_names.begin(), col_names.end(), "Мощность");
        has_field2 = std::count(col_names.begin(), col_names.end(), "Год_выпуска");
        if (has_field && has_field2 && Sample_reduction_by_TWO_field(copy_query2, ID_Type, col_names, type_car, unknown_cols, "Мощность", "Год_выпуска", stmtHandle, kol_in_db_Gaps_row)) {
            answer_query = SQLFetch(stmtHandle);
            continue;
        }
       
        //для TR попробовать убрать массы
        //тут продолжать вырезать поля

        answer_query = SQLFetch(stmtHandle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    return true;
}

void Delete_Null_Cols(const std::string& type_car) {
    SQLHANDLE stmtHandle2;
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle2, &stmtHandle2);//идентификатор запроса
    std::string delete_query = "DELETE FROM db3_mirror.dbo." + type_car + " where Количество = 0";
    auto a = SQLExecDirect(stmtHandle2, (SQLCHAR*)delete_query.c_str(), SQL_NTS);
    if (a != SQL_SUCCESS) {
        std::cout << "Ошибка удаления кортежа!";
        system("pause");
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle2);
}

bool Delete_copies_in_selection(SQLHANDLE& stmtHandle, const std::string& type_car, bool try_again = false) {
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle1, &stmtHandle);
    std::string query;
    if (!try_again) {
        query = "select DISTINCT ID_" + type_car + " from db3_mirror.." + type_car;
    }
    else {
        query = "select DISTINCT ID_" + type_car + " from db3_mirror.." + type_car + " where convert(float,Количество)<1";
    }
    auto a1 = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    SQLLEN cbColumnValue;
    SQLSMALLINT numCols;
    SQLNumResultCols(stmtHandle, &numCols);
    auto answer_query = SQLFetch(stmtHandle);
    if (answer_query == SQL_NO_DATA) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
        return false;
    }
    std::vector<std::string> vector_id;
    while (answer_query == SQL_SUCCESS) {  
        SQLCHAR columnValue[256];
        SQLGetData(stmtHandle, (1), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
        std::string columnValueString(reinterpret_cast<char*>(columnValue));
        vector_id.push_back(columnValueString);
        answer_query = SQLFetch(stmtHandle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    for (auto& current_id : vector_id) {
        SQLCHAR columnValue[256];
        std::string brand = "";
        std::string model = "";
        std::string year = "";
        std::string srez = "";
        SQLAllocHandle(SQL_HANDLE_STMT, connHandle3, &stmtHandle);
        if (!try_again) {
            query = "select sum(cast([Количество] as float)) from db3_mirror.." + type_car +
                " group by ID_" + type_car + " having ID_" + type_car + " = " + current_id;
        }
        else {
            query = "select * FROM db3_mirror.." + type_car + " where ID_" + type_car + "=" + current_id;
            SQLLEN cbColumnValue;
            SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
            auto answer_query = SQLFetch(stmtHandle);
            if (answer_query == SQL_NO_DATA) {
                SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
                continue;//ID могут удалиться при вычислении суммы у более раннего похожего кортежа, с вероятностью которого и суммируется вероятность удаленного ID 
            }
            SQLGetData(stmtHandle, (2), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString2(reinterpret_cast<char*>(columnValue));
            srez = columnValueString2;
            SQLGetData(stmtHandle, (3), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString(reinterpret_cast<char*>(columnValue));
            brand = columnValueString;
            SQLGetData(stmtHandle, (4), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString4(reinterpret_cast<char*>(columnValue));
            model = columnValueString4;
            SQLGetData(stmtHandle, (5), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);
            std::string columnValueString3(reinterpret_cast<char*>(columnValue));
            year = columnValueString3;
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
            query = "select SUM(convert(float,Количество)) FROM db3_mirror.." + type_car + " where ID_Марки = " + brand + " and ID_Модели = " + model + " and Год_выпуска = \'" + year + "\' and Срез_парка = \'" + srez + "\' and convert(float, Количество) < 1";
            SQLAllocHandle(SQL_HANDLE_STMT, connHandle3, &stmtHandle);
        }
        auto a = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
        if (a != SQL_SUCCESS) {
            show_error(SQL_HANDLE_STMT, stmtHandle);
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
            return false;
        }

        a = SQLFetch(stmtHandle);
        SQLGetData(stmtHandle, (1), SQL_C_CHAR, columnValue, sizeof(columnValue), &cbColumnValue);

        if (a != SQL_SUCCESS) {
            show_error(SQL_HANDLE_STMT, stmtHandle);
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
            return false;
        }
        std::string sum_kol(reinterpret_cast<char*>(columnValue));
        double d;
        std::istringstream(sum_kol) >> d;//для каста из string в double
        int res_kol = round(d);
        
        /*if (res_kol == 0 && try_again == false) {
            std::cout << d << "\n";
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
            continue;
        }*/
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
        if (!try_again) {
            Delete_row("DELETE FROM db3_mirror.dbo.", type_car, current_id);
        }
        else {
            SQLHANDLE stmtHandle2;
            SQLAllocHandle(SQL_HANDLE_STMT, connHandle2, &stmtHandle2);//идентификатор запроса
            std::string delete_query = "DELETE FROM db3_mirror.dbo." + type_car + " where ID_Марки = " + brand + " and ID_Модели = " + model + " and Год_выпуска = \'" + year + "\' and Срез_парка = \'" + srez + "\' and convert(float, Количество) < 1";
            auto a = SQLExecDirect(stmtHandle2, (SQLCHAR*)delete_query.c_str(), SQL_NTS);
            if (a != SQL_SUCCESS) {
                SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle2);
                std::cout << "Ошибка удаления кортежа!";
                system("pause");
            }
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle2);
        }
        //SQLAllocHandle(SQL_HANDLE_STMT, connHandle1, &stmtHandle);
        if (type_car == "PC") {
            query = "INSERT INTO db3_mirror..PC (ID_PC, Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность," +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_PC+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность,") +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..PC ") +
                std::string("WHERE ID_PC = ") + current_id;
        }
        else if (type_car == "BUS") {
            query = "INSERT INTO db3_mirror..BUS (ID_BUS, Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса," +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_BUS+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса,") +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..BUS ") +
                std::string("WHERE ID_BUS = ") + current_id;
        }
        else if (type_car == "HCV") {
            query = "INSERT INTO db3_mirror..HCV (ID_HCV, Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса," +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_расположения_руля, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_HCV+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса,") +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_расположения_руля, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..HCV ") +
                std::string("WHERE ID_HCV = ") + current_id;
        }
        else if (type_car == "LCV") {
            query = "INSERT INTO db3_mirror..LCV (ID_LCV, Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса," +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_LCV+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_вида_кузова, ID_типа_топлива, Объём_двигателя, Мощность, Полная_масса, Снаряжённая_масса,") +
                std::string("ID_типа_владельца, Экологический_класс, ID_места_производства, ID_страны_производства, ID_сегмента_автостата, ID_расположения_руля, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..LCV ") +
                std::string("WHERE ID_LCV = ") + current_id;
        }
        else if (type_car == "MT") {
            query = "INSERT INTO db3_mirror..MT (ID_MT, Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность," +
                std::string("ID_типа_владельца, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_MT+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, ID_типа_кузова, ID_типа_топлива, Объём_двигателя, Мощность,") +
                std::string("ID_типа_владельца, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..MT ") +
                std::string("WHERE ID_MT = ") + current_id;
        }
        /*if (type_car == "TR") {
            query = "INSERT INTO db3_mirror..TR (ID_TR, Срез_парка, ID_марки, ID_модели, Год_выпуска, Полная_масса, Снаряжённая_масса, ID_типа_прицепа, ID_типа_кузова_прицепа, Количество_осей_прицепа, Количество_шин_прицепа," +
                std::string("ID_типа_владельца, ID_округа,") +
                std::string("ID_района, Количество)") +
                std::string("SELECT (ID_TR+3000000), Срез_парка, ID_марки, ID_модели, Год_выпуска, Полная_масса, Снаряжённая_масса, ID_типа_прицепа, ID_типа_кузова_прицепа, Количество_осей_прицепа, Количество_шин_прицепа,") +
                std::string("ID_типа_владельца, ID_округа,") +
                std::string("ID_района,") + std::to_string(res_kol) + (" FROM db_Norm..TR ") +
                std::string("WHERE ID_TR = ") + current_id;
        }*/
      
        SQLAllocHandle(SQL_HANDLE_STMT, connHandle1, &stmtHandle);
        auto a1 = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
        if (a1 != SQL_SUCCESS) {
            std::cout << "Ошибка при удалении копий в выборке db_3! ID_" + type_car + " = " + current_id;
            return false;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    }
    return true;
}

void Make_Sample(SQLHANDLE& stmtHandle, std::vector<std::string> col_names, const std::string& type_car) {
    SQLAllocHandle(SQL_HANDLE_STMT, connHandle1, &stmtHandle);//идентификатор запроса
    if (Get_records_with_identical_columns("select * from db_Gaps_mirror.dbo.", type_car, std::move(col_names), stmtHandle, envHandle, connHandle1)) {
        std::cout << "Все неполные записи распределены по вероятности!";
    }
    else {
        std::cout << "НЕ все неполные записи распределены по вероятности!";
        system("pause");
    }
}

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus"); 
    
    SQLHANDLE stmtHandle;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);//Идентификатор окружения. Выделяется память и инициализируется дескриптор среды ODBC.
    SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_POINTER);//Устанавливается атрибут среды ODBC
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle1);//Идентификатор соединения. Выделяется память и инициализируется дескриптор подключения к базе данных 
    
    SQLRETURN ret;

    if (!Сonnect_DBMS(envHandle, connHandle1, ret)) {
        std::cout << "Не удалось установить первое подключение к SSMS." << std::endl;
        return 1;
    }
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle2);//второе соединение понадобится для получения результата запроса,
                                                            //когда предыдущий запрос не завершил свою работу
    if (!Сonnect_DBMS(envHandle, connHandle2, ret)) {
        std::cout << "Не удалось установить второе подключение к SSMS." << std::endl;
        return 1;
    }

    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle3);
    if (!Сonnect_DBMS(envHandle, connHandle3, ret)) {
        std::cout << "Не удалось установить второе подключение к SSMS." << std::endl;
        return 1;
    }

    //SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);//идентификатор запроса
    //if (!Create_DB(stmtHandle, ret, "db3")) {
    //    return 1;
    //}
    //SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    

    std::vector<std::string> col_name_PC { "ID_PC","Срез_парка","ID_марки","ID_модели","Год_выпуска","ID_типа_кузова",
                                          "ID_типа_топлива","Объём_двигателя","Мощность","ID_типа_владельца","Экологический_класс",
                                          "ID_места_производства","ID_страны_производства","ID_сегмента_автостата","ID_расположения_руля",
                                          "ID_округа","ID_района","Количество" };  
    
    Make_Sample(stmtHandle, col_name_PC, "PC");
    /*if (!Delete_copies_in_selection(stmtHandle, "PC")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для PC";
        return 1;
    }*/

    std::vector<std::string> col_name_BUS{ "ID_BUS","Срез_парка","ID_марки","ID_модели","Год_выпуска","ID_типа_кузова",
                                          "ID_типа_топлива","Объём_двигателя","Мощность","Полная_масса","Снаряжённая_масса",
                                          "ID_типа_владельца","Экологический_класс","ID_места_производства","ID_страны_производства",
                                          "ID_сегмента_автостата","ID_расположения_руля","ID_округа","ID_района","Количество" };

    Make_Sample(stmtHandle, col_name_BUS, "BUS");
    /*if (!Delete_copies_in_selection(stmtHandle, "BUS")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для BUS";
        return 1;
    }*/

    std::vector<std::string> col_name_HCV{ "ID_HCV","Срез_парка","ID_марки","ID_модели","Год_выпуска","ID_типа_кузова","ID_вида_кузова",
                                          "ID_типа_топлива","Объём_двигателя","Мощность","Полная_масса","Снаряжённая_масса",
                                          "ID_типа_владельца","Экологический_класс","ID_места_производства","ID_страны_производства",
                                          "ID_расположения_руля","ID_округа","ID_района","Количество" };

    Make_Sample(stmtHandle, col_name_HCV, "HCV");
    /*if (!Delete_copies_in_selection(stmtHandle, "HCV")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для HCV";
        return 1;
    }*/

    std::vector<std::string> col_name_LCV{ "ID_LCV","Срез_парка","ID_марки","ID_модели","Год_выпуска","ID_типа_кузова","ID_вида_кузова",
                                          "ID_типа_топлива","Объём_двигателя","Мощность","Полная_масса","Снаряжённая_масса",
                                          "ID_типа_владельца","Экологический_класс","ID_места_производства","ID_страны_производства",
                                          "ID_сегмента_автостата","ID_расположения_руля","ID_округа","ID_района","Количество" };

    Make_Sample(stmtHandle, col_name_LCV, "LCV");
    /*if (!Delete_copies_in_selection(stmtHandle, "LCV")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для LCV";
        return 1;
    }*/

    std::vector<std::string> col_name_MT{ "ID_MT","Срез_парка","ID_марки","ID_модели","Год_выпуска","ID_типа_кузова",
                                          "ID_типа_топлива","Объём_двигателя","Мощность","ID_типа_владельца",
                                          "ID_округа","ID_района","Количество" };

    Make_Sample(stmtHandle, col_name_MT, "MT");
    /*if (!Delete_copies_in_selection(stmtHandle, "MT")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для MT";
        return 1;
    }*/

    std::vector<std::string> col_name_TR{ "ID_TR","Срез_парка","ID_марки","ID_модели","Год_выпуска","Полная_масса","Снаряжённая_масса",
                                          "ID_типа_прицепа","ID_типа_кузова_прицепа","Количество_осей_прицепа","Количество_шин_прицепа",
                                          "ID_типа_владельца","ID_округа","ID_района","Количество" };

    //Make_Sample(stmtHandle, col_name_TR, "TR");
    /*if (!Delete_copies_in_selection(stmtHandle, "TR")) {
        std::cout << "Не удалось удалить копии кортежей и просуммировать их вероятности для TR";
        return 1;
    }*/

    //if (!Delete_copies_in_selection(stmtHandle, "PC", true)) {
    //    std::cout << "Не удалось удалить копии кортежей во второй попытке и просуммировать их вероятности для PC";
    //    return 1;
    //}
    //if (!Delete_copies_in_selection(stmtHandle, "BUS", true)) {
    //    std::cout << "Не удалось удалить копии кортежей во второй попытке и просуммировать их вероятности для BUS";
    //    return 1;
    //}
    //if (!Delete_copies_in_selection(stmtHandle, "HCV", true)) {
    //    std::cout << "Не удалось удалить копии кортежей во второй попытке и просуммировать их вероятности для HCV";
    //    return 1;
    //}
    //if (!Delete_copies_in_selection(stmtHandle, "LCV", true)) {
    //    std::cout << "Не удалось удалить копии кортежей во второй попытке и просуммировать их вероятности для LCV";
    //    return 1;
    //}
    //if (!Delete_copies_in_selection(stmtHandle, "MT", true)) {
    //    std::cout << "Не удалось удалить копии кортежей во второй попытке и просуммировать их вероятности для MT";
    //    return 1;
    //}

    
    //Delete_Null_Cols("BUS");
    //Delete_Null_Cols("HCV");
    //Delete_Null_Cols("LCV");
    //Delete_Null_Cols("MT");
    //Delete_Null_Cols("PC");
}