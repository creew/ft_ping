ALL_C =	icmp/chksum.c \
        queue/qping.c \
        logger.c \
        main.c \
        parse_arg.c \
        parse_ipv4.c \
        parse_options_scti.c \
        print_error.c \
        process_ping.c \
        statistics.c \
        usage.c \
        util.c

SRCDIR = ./srcs
OBJDIR = ./objs
OBJDIRS = $(OBJDIR) $(OBJDIR)/icmp $(OBJDIR)/queue
ALL_OBJ = $(ALL_C:%.c=%.o)

OBJS = $(addprefix $(OBJDIR)/, $(ALL_OBJ))

NAME = ft_ping

INCLUDES = ./includes/ft_ping.h

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): ./libft/libft.a $(OBJS)
	gcc $(FLAGS) -L./libft $(OBJS) -lft -lm -o $@

$(OBJDIRS):
	/bin/mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES) | $(OBJDIRS)
	gcc $(FLAGS) -I./includes -I./libft/includes -c $< -o $@

./libft/libft.a:
	make -C libft

clean:
	@/bin/rm -rf $(OBJDIR)
	make -C libft clean

fclean: clean
	@/bin/rm -rf $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: visu_hex lib ft_printf clean fclean all re
