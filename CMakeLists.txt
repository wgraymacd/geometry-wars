cmake_minimum_required(VERSION 3.31)
project(Game)

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# executable
add_executable(Game src/main.cpp)

# link SFML libs
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(Game PRIVATE sfml-graphics sfml-window sfml-system)

# copy runtime assets to binary output directory
add_custom_command(
    TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/bin
        $<TARGET_FILE_DIR:Game>
)