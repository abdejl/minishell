/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:49:41 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/18 20:49:50 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_input(t_redirect *redir, t_shell *shell)
{
	int		fd[2];
	char	*line;
	char	*expanded_line;

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		expanded_line = expand_heredoc_line(line, shell);
		ft_putendl_fd(expanded_line, fd[1]);
		free(line);
		free(expanded_line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	process_heredocs(t_cmd *cmd_list, t_shell *shell)
{
	t_redirect	*redir;

	while (cmd_list)
	{
		redir = cmd_list->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				redir->heredoc_fd = read_heredoc_input(redir, shell);
				if (redir->heredoc_fd < 0)
					return (-1);
			}
			redir = redir->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}

int	handle_heredoc(t_redirect *redir)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
