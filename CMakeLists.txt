cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror -O3)

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

set_target_properties(tinyhouse
    PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
)

target_compile_options(tinyhouse
    PRIVATE
      -Wall
      -Wextra
      -Werror
      -O3
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
      tests/move_generation.cpp
      tests/tiny_fen.cpp
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
