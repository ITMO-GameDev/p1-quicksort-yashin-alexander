set(target hwtest)

project("${META_PROJECT_NAME}-tests" C CXX)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(
    ../hwlib/include
    ${GTEST_INCLUDE_DIRS}
)

add_executable(hwtest ./sorttest.cpp ./arraytest.cpp)

target_link_libraries(hwtest ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
