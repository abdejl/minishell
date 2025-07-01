
#include "minishell.h"

/* التحقق مما إذا كان الحرف رقماً */
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && str[i] <= '9' && str[i] >= '0')
	{
		result = result * 10;
		result += str[i] - 48;
		i++;
	}
	return (result * sign);
}

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}


void ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
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

pid_t fork_process(void)
{
    pid_t pid = fork();
    if (pid == -1)
        ft_putstr_fd("minishell: fork", STDERR_FILENO);
    return pid;
}

void setup_child_process(t_shell *shell, t_cmd *cmd, int pipe_fd[2], int in_fd)
{
    int out_fd = cmd->next ? pipe_fd[1] : STDOUT_FILENO;
    
    if (in_fd != STDIN_FILENO)
        dup2(in_fd, STDIN_FILENO);
    if (out_fd != STDOUT_FILENO)
        dup2(out_fd, STDOUT_FILENO);
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
    int count = 0;
    while (cmd && ++count)
        cmd = cmd->next;
    return count;
}