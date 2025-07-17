#include "minishell.h"

static void	close_pipe_ends(int pipe_fd[2], t_cmd *cmd, int in_fd)
{
	if (cmd->next)
		close(pipe_fd[1]);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
}

static pid_t	fork_and_execute(t_shell *shell, t_cmd *cmd, int *in_fd)
{
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	last_pid = -1;

	while (cmd)
	{
		if (cmd->pipe_out)
			create_pipe(pipe_fd);
		pid = fork_process();
		if (pid == 0)
			setup_child_process(shell, cmd, pipe_fd, *in_fd);

		last_pid = pid; // ← نحفظ PID ديال آخر command

		close_pipe_ends(pipe_fd, cmd, *in_fd);

		if (cmd->pipe_out)
			*in_fd = pipe_fd[0];

		cmd = cmd->next;
	}
	return (last_pid); // ← نرجع آخر PID
}

int	wait_for_children(pid_t last_pid, t_shell *shell)
{
	int	status;

	waitpid(last_pid, &status, 0); // ← نسنى غير آخر command
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);

	return (shell->exit_status);
}
int	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	pid_t	last_pid;
	int		in_fd;
	int		result;

	in_fd = STDIN_FILENO;
	last_pid = fork_and_execute(shell, cmd, &in_fd); // ← نحصل على آخر PID
	result = wait_for_children(last_pid, shell);     // ← نخد exit_status الصحيح
	return (result);
}
