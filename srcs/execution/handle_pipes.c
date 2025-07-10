#include "minishell.h"

// static void	exec_child(t_shell *shell, t_cmd *cmd, int in_fd, int out_fd)
// {
// 	setup_child_fds(in_fd, out_fd);
// 	if (check_redirections(cmd) == -1)
// 		exit(EXIT_FAILURE);
// 	if (cmd->args && cmd->args[0])
// 	{
// 		if (is_builtin(cmd))
// 			exit(exec_builtin(shell, cmd));
// 		else
// 			exit(exec_external(shell, cmd));
// 	}
// 	exit(EXIT_SUCCESS);
// }

static void	close_pipe_ends(int pipe_fd[2], t_cmd *cmd, int in_fd)
{
	if (cmd->next)
		close(pipe_fd[1]);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
}

static int	fork_and_execute(t_shell *shell, t_cmd *cmd, pid_t *pids,
		int *in_fd)
{
	int	pipe_fd[2];
	int	i;

	i = 0;
	while (cmd)
	{
<<<<<<< HEAD
		if (cmd->pipe_out)
			create_pipe(pipe_fd);
=======
		if (cmd->next)
		{
			if(create_pipe(pipe_fd)== 1)
			shell->exit_status = 1;
			return(1);
		}
>>>>>>> 17f3197c24950f3272edccff81804e5422b5a3e8
		pids[i] = fork_process();
		if (pids[i] == 0)
			setup_child_process(shell, cmd, pipe_fd, *in_fd);
		close_pipe_ends(pipe_fd, cmd, *in_fd);
		if (cmd->pipe_out)
			*in_fd = pipe_fd[0];
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
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			last_status = WEXITSTATUS(status);
		i++;
	}
	shell->exit_status = last_status;
	return (0);
}

int	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	pid_t	*pids;
	int		cmd_count;
	int		in_fd;
	int		result;

	cmd_count = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);
	in_fd = STDIN_FILENO;
	fork_and_execute(shell, cmd, pids, &in_fd);
	result = wait_for_children(pids, cmd_count, shell);
	free(pids);
	return (result);
}