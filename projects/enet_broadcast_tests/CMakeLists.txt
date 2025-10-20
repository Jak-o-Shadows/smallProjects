cmake_minimum_required(VERSION 3.20)
project(EnetExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the asio package provided by Conan
find_package(asio REQUIRED)

# Common library for connection logic
add_library(common
    ${CMAKE_CURRENT_LIST_DIR}/src/connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/connection.hpp
)
target_include_directories(common PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(common PUBLIC asio::asio)
if(WIN32)
  # Target Windows 10 or later
  target_compile_definitions(common PUBLIC _WIN32_WINNT=0x0A00)
endif()

# Sender executable
add_executable(sender 
    ${CMAKE_CURRENT_LIST_DIR}/src/sender.cpp
)
target_link_libraries(sender common)
target_include_directories(sender PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Receiver executable
add_executable(receiver 
    ${CMAKE_CURRENT_LIST_DIR}/src/receiver.cpp
)
target_link_libraries(receiver common)
target_include_directories(receiver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
