/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:30:15 by abjellal          #+#    #+#             */
/*   Updated: 2025/04/22 14:13:46 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

t_cmd *new_command(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return cmd;
}

void cmd_add_redirect(t_cmd *cmd, int type, char *file)
{
    t_redirect *redir = malloc(sizeof(t_redirect));
    redir->type = type;
    redir->file = ft_strdup(file);
    redir->next = cmd->redirs;
    cmd->redirs = redir;
}

void	cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (cmd_list == NULL || new_cmd == NULL)
		return ;
	if (*cmd_list == NULL)
	{
		*cmd_list = new_cmd;
		return ;
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
