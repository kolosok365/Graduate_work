cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME Join_Tables)
project(${PROJECT_NAME})

include(FindODBC)
find_package(ODBC REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ODBC::ODBC)