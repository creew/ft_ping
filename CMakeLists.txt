cmake_minimum_required(VERSION 3.13)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libft)

include_directories(
        libft/includes
        includes)

add_executable(ft_ping
        src/main.c
        src/usage.c
        src/logger.c
        src/icmp/chksum.c
        src/parse_arg.c
        src/queue/qping.c
        src/statistics.c)
target_link_libraries(ft_ping ft)