cmake_minimum_required(VERSION 3.22)

project(CardRestApi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(CardRestApi
    Server.cpp
    CardHttpServer/CardHttpServer.cpp
    LENORMAND/LENORMAND.cpp
    TARO/TARO.cpp
)

target_include_directories(CardRestApi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CardRestApi PRIVATE
    Boost::system
    Boost::thread
    nlohmann_json::nlohmann_json
)

if (WIN32)
    target_compile_definitions(CardRestApi PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()