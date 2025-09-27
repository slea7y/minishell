# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/21 18:22:52 by maja              #+#    #+#              #
#    Updated: 2025/09/27 01:34:06 by tdietz-r         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

OBJDIR = obj


SRC = ./main.c \
	src/signals.c \
	src/parser/lexer/tokanisation.c \
	src/parser/lexer/init_env.c \
	src/parser/lexer/checker.c \
	src/parser/parsing/parsing.c \
	src/executor/utils.c \
	src/parser/lexer/handler.c \
	src/parser/lexer/token_to_struct.c \
	src/executor/start.c \
	src/parser/segmentation/segmentation.c \
	src/parser/segmentation/init.c \
	src/parser/segmentation/segmentation_handler.c \
	src/parser/segmentation/expansion.c \
	src/parser/lexer/init.c \
	src/parser/parsing/variable_expansion.c \
	src/parser/parsing/line_reader.c \
	src/parser/parsing/heredoc_processor.c \
	src/parser/validation/syntax_validation.c \
	src/executor/builtins/builtin_cd.c \
	src/executor/builtins/builtin_commands.c \
	src/executor/builtins/builtin_unset.c \
	src/executor/builtins/export_utils.c \
	src/executor/builtins/pipes.c \
	src/executor/builtins/process.c \
	src/executor/builtins/redirections.c \
	
OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes
LINKER = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME) $(LINKER)
	@echo "Done!"

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

test: clean
	@echo "Running minishell tester..."
	@bash 42_minishell_tester/tester.sh m
	@echo "Cleaning up tester files..."
	@$(MAKE) cleanup
	@echo "Tester cleanup complete!"

cleanup:
	@echo "Cleaning up tester files..."
	@rm -f tmp_* 0 42 '<' '>' PATH 'PATH=' export unset bla_test chmod 1 -r -f
	@rm -f '%s\n' ls rm touch 'minishe*l' '*a' '*a*a' '**' '***'
	@rm -f *.txt *.log *.out *.err
	@rm -f a.out core core.*
	@rm -f test_* debug_* temp_*
	@echo "Tester cleanup complete!"

.PHONY: all clean fclean re test cleanup