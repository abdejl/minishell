#ifndef MINISHEL_H
#define MINISHEL_H

#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include "../libft/libft.h"

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_IN,
    TOKEN_HEREDOC,
    TOKEN_APPEND
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

int is_white_space(char c);
int is_quote(char *str);
void skip_white_spaces(char **str);
char *extract_quoted_string(char **input);
int is_operator(char *str);
void add_token(t_token **token_list, char *value, int type);
void free_tokens(t_token *tokens);
char *extract_word(char **str);
char *extract_operator(char **str);
int get_operator_type(char *op);
char    *ft_strndup(const char *s, size_t n);


typedef struct s_cmd {
    char        **args;      // Array of command + arguments (e.g., ["echo", "hello"])
    t_redirect  *redirs;     // List of redirections
    struct s_cmd *next;      // Next command in pipe (NULL if last command)
} t_cmd;

typedef struct s_redirect {
    char    *file;          // File name (e.g., "output.txt")
    int     type;           // TYPE_IN, TYPE_OUT, TYPE_APPEND, TYPE_HEREDOC
    struct s_redirect *next; 
} t_redirect;

#endif