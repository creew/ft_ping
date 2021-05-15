cmake_minimum_required(VERSION 3.16)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libft)

include_directories(
        libft/includes
        includes)

add_executable(ft_ping src/main.c src/usage.c src/logger.c)
target_link_libraries(ft_ping ft)