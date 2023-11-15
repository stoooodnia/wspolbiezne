cmake_minimum_required(VERSION 3.26)
project(server)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)