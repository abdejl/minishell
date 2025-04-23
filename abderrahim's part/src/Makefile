NAME        = minishell
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I$(INCLUDES) -I$(LIBFT_DIR)
INCLUDES    = includes
LIBFT_DIR   = ../libft
LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = $(LIBFT)

SRC_DIR     = src
SRCS        = parsing.c utils.c utils2.c utils_3.c utils_4.c free_utils.c
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
