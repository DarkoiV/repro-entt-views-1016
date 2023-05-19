cmake_minimum_required(VERSION 3.22) 
project(repro_entt_view)
include(FetchContent)

FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        fef921132cae7588213d0f9bcd2fb9c8ffd8b7fc # 3.11.1
)
FetchContent_MakeAvailable(Entt)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
add_executable(repro
    main.cpp)
target_link_libraries(repro
    EnTT::EnTT)
set_property(TARGET repro PROPERTY CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Wpedantic)

