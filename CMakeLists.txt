cmake_minimum_required(VERSION 3.13)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libft)

include_directories(
        libft/includes
        includes)

add_executable(ft_ping
        srcs/usage.c
        srcs/logger.c
        srcs/parse_ipv4.c
        srcs/util.c
        srcs/statistics.c
        srcs/parse_arg.c
        srcs/queue/qping.c
        srcs/process_ping.c
        srcs/main.c
        srcs/icmp/chksum.c
        srcs/parse_options_scti.c
        srcs/print_error.c)
target_link_libraries(ft_ping ft m)