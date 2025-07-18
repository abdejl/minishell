/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:07:46 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/09 10:07:46 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_empty_args(t_cmd *cmd)
{
	int		read_idx;
	int		write_idx;
	char	**args;

	if (!cmd || !cmd->args)
		return ;
	args = cmd->args;
	read_idx = 0;
	while (args[read_idx] && args[read_idx][0] == '\0')
	{
		free(args[read_idx]);
		read_idx++;
	}
	write_idx = 0;
	while (args[read_idx])
	{
		args[write_idx] = args[read_idx];
		read_idx++;
		write_idx++;
	}
	while (write_idx < read_idx)
	{
		args[write_idx] = NULL;
		write_idx++;
	}
}

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
		cleanup_empty_args(current_cmd);
		current_cmd = current_cmd->next;
	}
}
