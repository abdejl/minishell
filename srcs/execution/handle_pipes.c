/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:34:10 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/19 10:35:35 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief In the parent process, closes the pipe ends that are no longer needed.
 */
static void	parent_close_fds(t_cmd *cmd, int pipe_fd[2], int *in_fd)
{
	// Close the previous pipe's read end, as it's now used by the child
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	// If there's a next command, set up the input for it
	if (cmd->pipe_out)
	{
		close(pipe_fd[1]); // Parent doesn't need the write end
		*in_fd = pipe_fd[0];
	}
}

/**
 * @brief The main loop to fork each process in the pipeline.
 * It now stores every created PID into the `pids` array.
 */
static int	fork_and_execute(t_shell *shell, t_cmd *cmd, pid_t *pids, int *in_fd)
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
	return (i + 1); // Return the total number of forked processes
}

/**
 * @brief Waits for all child processes in the pipeline to complete.
 */
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
	// Update the shell's exit status based on the LAST command's result
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}

/**
 * @brief The main entry point for executing a pipeline.
 */
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

// int	execute_pipeline(t_shell *shell, t_cmd *cmd)
// {
// 	pid_t	last_pid;
// 	int		in_fd;
// 	int		result;

// 	in_fd = STDIN_FILENO;
// 	last_pid = fork_and_execute(shell, cmd, &in_fd);
// 	result = wait_for_children(last_pid, shell);
// 	return (result);
// }
