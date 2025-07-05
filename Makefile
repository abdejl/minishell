NAME = minishell

# Paths
INCLUDES = includes/
LIBFT_PATH = libft/
GNL_PATH = gnl/

# Sources
SRCS = srcs/main.c \
       srcs/builtins/cd.c \
       srcs/builtins/echo.c \
       srcs/builtins/env.c \
       srcs/builtins/exit.c \
       srcs/builtins/export.c \
       srcs/builtins/pwd.c \
       srcs/builtins/unset.c \
       srcs/execution/executor.c \
       srcs/execution/handle_cmd.c \
       srcs/execution/handle_pipes.c \
       srcs/execution/check_redirections.c \
       srcs/execution/env_utils.c\
       srcs/parsing/lexer.c \
       srcs/parsing/parser.c \
       srcs/parsing/parsing_utils.c \
       srcs/parsing/utils.c \
       srcs/parsing/utils2.c \
       srcs/parsing/utils_3.c \
       srcs/parsing/utils_4.c \
       srcs/parsing/expander_utils.c \
       srcs/parsing/expander_utils_2.c \
       srcs/parsing/expander.c \
       srcs/utils/signal_handler.c \
       srcs/utils/builtins_utils.c \
       srcs/utils/free_utils.c

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror
# Add these to your Makefile
READLINE_INC = -I/usr/include -I/usr/local/opt/readline/include
READLINE_LIB = -L/usr/lib -L/usr/local/opt/readline/lib -lreadline
READLINE_FLAGS = -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft $(READLINE_FLAGS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -I$(LIBFT_PATH) $(READLINE_FLAGS) -c $< -o $@

clean:
	@make -C $(LIBFT_PATH) clean
	@rm -f $(OBJS)
	@echo "$(RED)🧹 Object files removed!$(RESET)"

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@rm -f $(NAME)
	@echo "$(RED)🧹 $(NAME) removed!$(RESET)"

re: fclean all

norm:
	@norminette $(SRCS) $(INCLUDES)*.h
	@make -C $(LIBFT_PATH) norm

.PHONY: all clean fclean re norm