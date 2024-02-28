cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME Create_connect_filling_2db)
project(${PROJECT_NAME})

include(FindODBC)
find_package(ODBC REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/OpenXLSX/OpenXLSX)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenXLSX::OpenXLSX
ODBC::ODBC
)


