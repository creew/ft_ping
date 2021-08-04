cmake_minimum_required(VERSION 3.13)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libft)

include_directories(
        libft/includes
        includes)

add_executable(ft_ping
        src/usage.c
        src/logger.c
        src/parse_ipv4.c
        src/util.c
        src/statistics.c
        src/parse_arg.c
        src/queue/qping.c
        src/process_ping.c
        src/main.c
        src/icmp/chksum.c
        src/parse_options_scti.c
        src/print_error.c)
target_link_libraries(ft_ping ft m)