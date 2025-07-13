#include "minishell.h"

pid_t fork_process(void)
{
    pid_t pid = fork();
    if (pid == -1)
        ft_putstr_fd("minishell: fork", STDERR_FILENO);
    return pid;
}

void setup_child_process(t_shell *shell, t_cmd *cmd, int pipe_fd[2], int in_fd)
{
	int	out_fd;

	if (cmd->next)
		out_fd = pipe_fd[1];
	else
		out_fd = STDOUT_FILENO;
	
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (cmd->next)
		close(pipe_fd[0]);
	if (check_redirections(cmd) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd))
		exit(exec_builtin(shell, cmd));
	exit(exec_external(shell, cmd));
}

int count_commands(t_cmd *cmd)
{
    int count;

    count = 0;
    while (cmd)
    {
        count++;
        if (!cmd->pipe_out)
            break;
        cmd = cmd->next;
    }
    return (count);
}

int create_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
    {
        ft_putstr_fd("minishell: pipe", STDERR_FILENO);
        return 1;
    }
    return 0;
}