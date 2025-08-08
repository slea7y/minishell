# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 22:35:15 by majkijew          #+#    #+#              #
#    Updated: 2025/08/08 22:00:39 by majkijew         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# SRC = builtin_commands.c builtin_cd.c env_init.c utils.c export_utils.c #for testing
SRC = builtin_commands.c builtin_cd.c env_init.c utils.c mini_main.c \
	export_utils.c builtin_unset.c
OBJS = $(SRC:%.c=%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
LINKER = -lreadline

LIBFT_DIR = /Users/majkijew/Documents/4circle/minishell/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LINKER)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) clean

re: fclean all

.PHONY: all clean fclean re
