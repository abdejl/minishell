/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 12:06:28 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/20 12:06:28 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

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
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_ERROR,
	TOKEN_HEREDOC,
	TOKEN_SEMICOLON,
	TOKEN_AMPERSAND,
	TOKEN_APPEND
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redirect
{
	char				*file;
	int					type;
	int					heredoc_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	char				**args;
	t_redirect			*redirs;
	int					pipe_out;
	int					background;
	struct s_cmd		*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env_list;
	int		exit_status;
}	t_shell;

typedef struct s_str_list
{
	char				*str;
	struct s_str_list	*next;
}	t_str_list;

typedef struct s_expand_state
{
	t_str_list	**list;
	char		*p;
	char		*segment_start;
	int			in_s_quotes;
	int			in_d_quotes;
	t_shell		*shell;
}	t_expand_state;

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

/* ========== FUNCTION PROTOTYPES ========== */

/* ---------- Parsing ---------- */

/* Lexer */

t_token		*lexer(char *input, t_shell *shell);
int			is_white_space(char c);
void		skip_white_spaces(char **str);
int			is_quote(char c);
char		*extract_quoted_string(char **input, t_shell *shell);
t_token		*handle_quoted_string(char **input, t_token *tokens);
t_token		*handle_operator(char **input, t_token *tokens);
t_token		*handle_word(char **input, t_token *tokens);
int			handle_operator_token(char **input, t_token **list);
int			is_operator(char c);
char		*extract_word(char **str);
char		*extract_operator(char **str);
int			get_operator_type(char *op);
void		add_token(t_token **token_list, char *value, t_token_type type);

/* Parser */
t_cmd		*parser(t_token *tokens, t_shell *shell);
t_cmd		*new_command(void);
void		cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd);
int			is_pipe(t_token *token);
int			is_redirect(t_token *token);
int			handle_pipe(t_token **tokens, t_cmd **cmd_list, t_cmd **current);
int			handle_redirect(t_token **tokens, t_cmd **cmd_list, t_cmd *current);
void		add_arg(t_cmd *cmd, char *arg);
int			add_redirect(t_cmd *cmd, int type, char *file);
void		print_syntax_error(char *token);
void		expander(t_cmd *cmds, t_shell *shell);
char		*get_var_value(char **str_ptr, t_shell *shell);
void		append_str_node(t_str_list **list, char *str);
char		*join_str_list(t_str_list *list);
void		free_str_list(t_str_list *list);

/* ---------- Execution ---------- */

int			executor(t_shell *shell, t_cmd *cmd_list);
int			handle_cmd(t_shell *shell, t_cmd *cmd);
int			is_builtin(t_cmd *cmd);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			exec_external(t_shell *shell, t_cmd *cmd);
int			execute_pipeline(t_shell *shell, t_cmd *cmd);
int			check_redirections(t_cmd *cmd);
int			handle_heredoc(t_redirect *redir);
int			process_heredocs(t_cmd *cmd_list, t_shell *shell);
char		*expand_heredoc_line(char *line, t_shell *shell);

/* ---------- Builtins ---------- */

int			ft_echo(t_cmd *cmd);
int			cd_builtin(t_shell *shell, t_cmd *cmd);
int			ft_env(t_env *env);
int			ft_export(t_shell *shell, t_cmd *cmd);
int			ft_unset(t_shell *shell, t_cmd *cmd);
int			mini_exit(t_shell *shell, t_cmd *cmd);
void		ft_putstr_fd(char *s, int fd);
int			ft_pwd(void);

/*-------export utils------*/

void		split_key_value(char *arg, char **key, char **value);
void		print_exported_env(t_env *env_list);
int			is_valid_identifier(char *key);
int			handle_invalid_key(char *key, t_shell *shell);

/*--------Expander---------*/

int			is_expandable(char c);
void		process_quote(t_expand_state *state);
void		process_dollar(t_expand_state *state);
void		init_expand_state(t_expand_state *st, char *arg, t_shell *sh);
char		*expand_and_join(char *arg, t_shell *shell);

/* ---------- Environment ---------- */

t_env		*init_env_list(char **envp);
char		*get_env_value(t_env *env, char *key);
int			set_env_value(t_env *env, char *key, char *value);
int			save_oldpwd(char *cwd, char **oldpwd);
void		add_env(t_env **env_list, char *key, char *value);
t_env		*get_env_node(t_env *env_list, char *key);

/* ---------- Utils ---------- */

void		free_tokens(t_token *tokens);
void		free_cmds(t_cmd *cmds);
void		free_cmd(t_cmd *cmd);
void		free_redirect(t_redirect *redir);
void		ft_free_env_list(t_env *env);
char		*ft_strndup(const char *s, size_t n);
char		*strjoin_free(char *s1, char *s2);
char		**env_to_array(t_env *env_list);
void		free_str_list(t_str_list *list);
void		*gc_malloc(size_t size);
void		gc_free_all(void);
void		print_error(char *prefix, char *target, char *message);

/* ---------- Signals ---------- */

void		set_signal_parent(void);
void		set_signal_child(void);
void		sigint_handler(int sig);
void		update_exit_status(t_shell *shell, int status);
void		free_str_array(char **array);

/* ---------- Debug ---------- */

int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);

void		handle_execve_error(char *path, char **envp);
void		exec_child(t_shell *shell, t_cmd *cmd);

int			create_pipe(int pipe_fd[2]);
pid_t		fork_process(void);
void		setup_child_process(t_shell *shell, t_cmd *cmd, int pipe_fd[2],
				int in_fd);
int			count_commands(t_cmd *cmd);
#endif