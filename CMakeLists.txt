cmake_minimum_required(VERSION 3.10)
project(test1)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

qt5_wrap_cpp (MOC_SOURCES ${HEADERS})

add_executable(test1 main.cpp main.h)

qt5_use_modules(${PROJECT_NAME} Widgets Core Gui)
#target_link_libraries(${PROJECT_NAME} Qt5::Core)
#target_link_libraries(${PROJECT_NAME} Qt5::Gui)
#target_link_libraries(${PROJECT_NAME} Qt5::Widgets)