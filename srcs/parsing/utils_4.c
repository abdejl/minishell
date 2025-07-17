/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:29:58 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/17 21:58:43 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int is_redirect(t_token *token)
{
    if (!token)
        return (0);
    return (token->type == TOKEN_REDIR_IN || 
            token->type == TOKEN_REDIR_OUT ||
            token->type == TOKEN_APPEND ||
            token->type == TOKEN_HEREDOC);
}

void add_arg(t_cmd *cmd, char *arg)
{
    int     i;
    char    **new_args;

    if (!cmd || !arg)
        return;
    i = 0;
    while (cmd->args && cmd->args[i])
        i++;
    new_args = (char **)gc_malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;
    i = 0;
    while (cmd->args && cmd->args[i])
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

int add_redirect(t_cmd *cmd, int type, char *file)
{
    t_redirect *new_redir;

    if (!cmd || !file)
        return 0;
    new_redir = (t_redirect *)gc_malloc(sizeof(t_redirect));
    if (!new_redir)
        return 0;
    new_redir->type = type;
    new_redir->file = ft_strdup(file);
    if (!new_redir->file)
    {
        free(new_redir);
        return 0;
    }
    new_redir->next = cmd->redirs;
    cmd->redirs = new_redir;
    return 1;
}
