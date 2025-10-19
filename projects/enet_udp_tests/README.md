



# Project Prompt
Using PURELY the enet_udp_tests folder, make a example of sending and receiving data via enet. It should be in C++, using conan to get the packages. There should be a common Connection class in connection.hpp, with receiver.cpp and sender.cpp being the example applications. The CMakeLists.txt should specify three build targets: a common library that contaisn Connection.cpp, and the sender and receiver application. Use C++20. Keep everything as simple as possible.

Do not create, delete, or move files. The folder structure is perfect as is.

All source code, includign headers and CPP files, belong in the cpp folder. e.g. the files are:
 * conanfile.py
 * CMakeLists.txt
 * src/connection.hpp
 * src/connection.cpp
 * src/receiver.cpp
 * src/sender.cpp

To make the applications/libraries:
 * sender.exe
 * receiver.exe
 * common.lib

I should be able to build the project with `conan build .`