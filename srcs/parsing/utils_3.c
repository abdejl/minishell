/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:15 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/17 21:59:25 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *new_command(void)
{
    t_cmd *cmd = gc_malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    cmd->pipe_out = 0;
    return cmd;
}

int cmd_add_redirect(t_cmd *cmd, int type, char *file)
{
    t_redirect *redir;
    
    if (!cmd || !file)
        return (0);
    redir = gc_malloc(sizeof(t_redirect));
    if (!redir)
        return (0);
    redir->type = type;
    redir->file = ft_strdup(file);
    if (!redir->file)
    {
        free(redir);
        return (0);
    }
    redir->next = cmd->redirs;
    cmd->redirs = redir;
    return (1);
}

void cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
    t_cmd *tmp;

    if (!cmd_list || !new_cmd)
        return;
    if (*cmd_list == NULL)
    {
        *cmd_list = new_cmd;
        return;
    }
    tmp = *cmd_list;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = new_cmd;
}

int is_pipe(t_token *token)
{
    return (token && token->type == TOKEN_PIPE);
}

int    ft_strcmp(const char *s1, const char *s2)
{
    size_t    i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
