#CMake version required
cmake_minimum_required(VERSION 3.20) 

#Name of the project
project(weather-api) 

#C++ Version required
set(CMAKE_CXX_STANDARD 20) 
#Program is required to have this version, not suggesting.
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

#Lets CMake look into root folders for libraries
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
endif()

#This package is required else the program will not run.
find_package(CURL REQUIRED)

#The exectuable file generated will be called weather-api and is generated from src/main.cpp
add_executable(weather-api
    src/main.cpp          
)

#CMake will link the target libraries to weather-api
target_link_libraries(weather-api PRIVATE CURL::libcurl)