/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:24:58 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/06 10:24:58 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_args(t_cmd *cmd, t_shell *shell)
{
	char	*expanded_str;
	int		i;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded_str = expand_and_join(cmd->args[i], shell);
		free(cmd->args[i]);
		if (expanded_str)
			cmd->args[i] = expanded_str;
		else
			cmd->args[i] = ft_strdup("");
		i++;
	}
}

static void	expand_redirs(t_cmd *cmd, t_shell *shell)
{
	t_redirect	*redir;
	char		*expanded_str;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != TOKEN_HEREDOC)
		{
			expanded_str = expand_and_join(redir->file, shell);
			free(redir->file);
			if (expanded_str)
				redir->file = expanded_str;
			else
				redir->file = ft_strdup("");
		}
		redir = redir->next;
	}
}

void	expander(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current_cmd;

	current_cmd = cmds;
	while (current_cmd)
	{
		expand_args(current_cmd, shell);
		expand_redirs(current_cmd, shell);
		current_cmd = current_cmd->next;
	}
}
