cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Tinyhouse)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(tinyhouse)

target_sources(tinyhouse
    PRIVATE
      src/board.cpp
      src/game_manager.cpp
      src/game.cpp
      src/house.cpp
      src/main.cpp
      src/move.cpp
      src/renderer.cpp
)

target_link_libraries(tinyhouse
    PRIVATE
      SDL3::SDL3
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.16.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests)

target_sources(tests
    PRIVATE
      tests/tiny_fen_test.cpp
      tests/move_test.cpp
      src/board.cpp
      src/game.cpp
      src/house.cpp
      src/move.cpp
)

target_include_directories(tests PRIVATE inc)

target_link_libraries(tests
    PRIVATE
      Catch2::Catch2WithMain
      SDL3::SDL3
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
