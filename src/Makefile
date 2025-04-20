NAME        = minishell
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
INCLUDES    = includes
#LIBFT       = libft/libft.a

SRC_DIR     = src
SRCS        = parsing.c utils.c utils2.c
OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = libft

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
#	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
#	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re