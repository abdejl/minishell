NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = ft_atoi.c ft_memset.c ft_bzero.c ft_putchar_fd.c ft_calloc.c ft_putendl_fd.c \
      ft_isalnum.c ft_putnbr_fd.c ft_isalpha.c ft_putstr_fd.c ft_isascii.c ft_split.c \
      ft_isdigit.c ft_strchr.c ft_isprint.c ft_strdup.c ft_itoa.c ft_striteri.c \
      ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
      ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_memchr.c ft_substr.c ft_memcmp.c \
      ft_tolower.c ft_memcpy.c ft_toupper.c ft_memmove.c

BSRC = ft_lstadd_back_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstdelone_bonus.c \
       ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstnew_bonus.c \
       ft_lstiter_bonus.c ft_lstmap_bonus.c

NAME = libft.a
CC = cc 
FLAGS = -Wall - Wextra - Werror 

RM = rm -f 
AR = ar rcs
OBJ = $(SRC:%.c=%.o)
BOBJ = $(BSRC:%.c=%.o)

all: $(NAME)
bonus: $(BOBJ)
	$(AR) $(NAME) $^
	@touch bonus

$(NAME):$(OBJ)
	$(AR) $(NAME) $^
clean:
	$(RM) $(OBJ) $(BOBJ) bonus
fclean: clean
	$(RM) $(NAME)
re: fclean all clean re
