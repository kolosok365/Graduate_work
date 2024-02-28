cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDART 20)
set(PROJECT_NAME Recovery_db)
project(${PROJECT_NAME})

include(FindODBC)
find_package(ODBC REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ODBC::ODBC)