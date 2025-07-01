#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

/* ========== CONSTANTS ========== */
# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define SUCCESS 0
# define ERROR 1
# define EXIT_ERROR 1
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define FILE_PERMISSIONS 0644

/* ========== STRUCTURES ========== */
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_IN,
    TOKEN_ERROR,
    TOKEN_HEREDOC,
    TOKEN_APPEND
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef struct s_redirect {
    char    *file;
    int     type;
    struct s_redirect *next; 
} t_redirect;

typedef struct s_cmd {
    char        **args;
    t_redirect  *redirs;
    int         pipe_out;
    struct s_cmd *next; 
} t_cmd;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_shell {
    t_env *env_list;
    int exit_status;
} t_shell;

/* ========== FUNCTION PROTOTYPES ========== */

/* ---------- Parsing ---------- */
/* Lexer */
t_token *lexer(char *input);
int     is_white_space(char c);
void    skip_white_spaces(char **str);
int     is_quote(char *str);
char    *extract_quoted_string(char **input);
t_token	*handle_quoted_string(char **input, t_token *tokens);
t_token	*handle_operator(char **input, t_token *tokens);
t_token	*handle_word(char **input, t_token *tokens);
int     is_operator(char *str);
char    *extract_word(char **str);
char    *extract_operator(char **str);
int     get_operator_type(char *op);
void    add_token(t_token **token_list, char *value, int type);

/* Parser */
t_cmd   *parser(t_token *tokens);
t_cmd   *new_command(void);
void    cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd);
int     is_pipe(t_token *token);
int     is_redirect(t_token *token);
int     handle_pipe(t_token **tokens, t_cmd **cmd_list, t_cmd **current);
int     handle_redirect(t_token **tokens, t_cmd **cmd_list, t_cmd *current);
void    add_arg(t_cmd *cmd, char *arg);
int     add_redirect(t_cmd *cmd, int type, char *file);
void print_syntax_error(char *token);

/* ---------- Execution ---------- */
int     executor(t_shell *shell, t_cmd *cmd_list);
int     handle_cmd(t_shell *shell, t_cmd *cmd);
int     check_redirections(t_cmd *cmd);
int     is_builtin(t_cmd *cmd);
int     exec_builtin(t_shell *shell, t_cmd *cmd);
int     exec_external(t_shell *shell, t_cmd *cmd);
int     execute_pipeline(t_shell *shell, t_cmd *cmd);
int check_redirections(t_cmd *cmd);
int handle_heredoc(t_redirect *redir);

/* ---------- Builtins ---------- */
int     ft_echo(t_cmd *cmd);
int     cd_builtin(t_shell *shell, t_cmd *cmd);
int     ft_env(t_env *env);
int     ft_export(t_shell *shell, t_cmd *cmd);
void    ft_unset(t_env **env_list, char *key);
int     mini_exit(t_shell *shell, t_cmd *cmd);
void    ft_putstr_fd(char *s, int fd);
int     ft_pwd(void);


/* ---------- Environment ---------- */
t_env   *init_env_list(char **envp);
char    *get_env_value(t_env *env, char *key);
int     set_env_value(t_env *env, char *key, char *value);
void    add_env(t_env **env_list, char *key, char *value);
int     is_valid_identifier(char *key);
t_env *get_env_node(t_env *env_list, char *key);

/* ---------- Utils ---------- */
void    free_tokens(t_token *tokens);
void    free_cmds(t_cmd *cmds);
void    free_cmd(t_cmd *cmd);
void    free_redirect(t_redirect *redir);
void    ft_free_env_list(t_env *env);
char    *ft_strndup(const char *s, size_t n);
char    *strjoin_free(char *s1, char *s2);
char    **env_to_array(t_env *env_list);

/* ---------- Signals ---------- */
void    set_signal_parent(void);
void    set_signal_child(void);
void    sigint_handler(int sig);
void    update_exit_status(t_shell *shell, int status);
void    free_str_array(char **array);
void    set_child_signals(void);
void    set_signals(void);

/* ---------- Debug ---------- */
void    print_parsed_cmds(t_cmd *cmds);
void    print_tokens(t_token *tokens);
int     ft_strcmp(const char *s1, const char *s2);


int create_pipe(int pipe_fd[2]);
pid_t fork_process(void);
void setup_child_process(t_shell *shell, t_cmd *cmd, int pipe_fd[2], int in_fd);
int count_commands(t_cmd *cmd);
#endif