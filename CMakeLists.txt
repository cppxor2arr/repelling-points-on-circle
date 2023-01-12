cmake_minimum_required(VERSION 3.20)
project(RepellingPointsOnCircle VERSION 0.1.0 LANGUAGES CXX)

add_executable(RepellingPointsOnCircle
    src/main.cpp
    src/spin.cpp
    src/helpers.cpp)

target_compile_features(RepellingPointsOnCircle PRIVATE cxx_std_17)
set_target_properties(RepellingPointsOnCircle PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(RepellingPointsOnCircle PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)

target_include_directories(RepellingPointsOnCircle PRIVATE include)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

target_link_libraries(RepellingPointsOnCircle PRIVATE sfml-graphics)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET RepellingPointsOnCircle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:RepellingPointsOnCircle> $<TARGET_FILE_DIR:RepellingPointsOnCircle> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS RepellingPointsOnCircle)
