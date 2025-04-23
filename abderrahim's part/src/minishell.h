/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:03 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/22 14:14:55 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
#define MINISHEL_H

#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include "../libft/libft.h"

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_IN,
    TOKEN_HEREDOC,
    TOKEN_APPEND
} t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef struct s_redirect
{
    char    *file;
    int     type;
    struct s_redirect *next; 
} t_redirect;

typedef struct s_cmd
{
    char        **args;
    t_redirect  *redirs;
    struct s_cmd *next; 
} t_cmd;

int     is_white_space(char c);
int     is_quote(char *str);
void    skip_white_spaces(char **str);
char    *extract_quoted_string(char **input);
int     is_operator(char *str);
void    add_token(t_token **token_list, char *value, int type);
void    free_tokens(t_token *tokens);
char    *extract_word(char **str);
char    *extract_operator(char **str);
int     get_operator_type(char *op);
char    *ft_strndup(const char *s, size_t n);

void	cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd);
void    cmd_add_redirect(t_cmd *cmd, int type, char *file);
t_cmd   *new_command(void);
int     is_pipe(t_token *token);
void    add_redirect(t_cmd *cmd, int type, char *file);
int     is_redirect(t_token *token);
void    add_arg(t_cmd *cmd, char *arg);
void    free_cmds(t_cmd *cmds);
void    free_redirect(t_redirect *redir);

#endif