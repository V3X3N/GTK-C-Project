cmake_minimum_required(VERSION 3.5)
project(GTKLibrary)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(GTK3_INCLUDE_DIRS "C:/msys64/mingw64/include/")
set(GTK3_LIBRARY_DIRS "C:/msys64/mingw64/lib")
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(GTKProject main.c)
set_target_properties(GTKProject PROPERTIES WIN32_EXECUTABLE true)  # If you dont want console to run with your .exe
target_link_libraries(GTKProject ${GTK4_LIBRARIES})