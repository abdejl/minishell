/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:34:10 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/20 10:18:25 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parent_close_fds(t_cmd *cmd, int pipe_fd[2], int *in_fd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->pipe_out)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

static int	fork_and_execute(t_shell *shell, t_cmd *cmd, pid_t *pids,
	int *in_fd)
{
	int	pipe_fd[2];
	int	i;

	i = 0;
	while (cmd)
	{
		if (cmd->pipe_out)
			create_pipe(pipe_fd);
		pids[i] = fork_process();
		if (pids[i] == 0)
			setup_child_process(shell, cmd, pipe_fd, *in_fd);
		parent_close_fds(cmd, pipe_fd, in_fd);
		if (!cmd->pipe_out)
			break ;
		cmd = cmd->next;
		i++;
	}
	return (i + 1);
}

static int	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	status;
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}

int	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	pid_t	*pids;
	int		cmd_count;
	int		in_fd;

	cmd_count = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);
	in_fd = STDIN_FILENO;
	fork_and_execute(shell, cmd, pids, &in_fd);
	wait_for_children(pids, cmd_count, shell);
	free(pids);
	return (shell->exit_status);
}
