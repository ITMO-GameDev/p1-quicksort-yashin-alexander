cmake_minimum_required(VERSION 3.5)

project(hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_subdirectory(src)


target_link_libraries(main
    ${DEFAULT_LIBRARIES}
    ${META_PROJECT_NAME}::hwlib
)
