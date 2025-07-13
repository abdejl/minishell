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
		// FIX: Call the new, dedicated heredoc expander
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

int handle_input_redirect(t_redirect *redir)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	if(fd < 0)
	{
        ft_putstr_fd(redir->file, STDERR_FILENO);
			ft_putstr_fd(": no such file or directory\n", STDERR_FILENO);
			return(-1);
	}
	dup2(fd , STDIN_FILENO);
	close(fd);
	return(0);
}

int handle_output_redirect(t_redirect *redir)
{
	int fd;

	fd = open(redir->file , O_WRONLY | O_CREAT | O_TRUNC , 0644 );
	if(fd < 0)
	{
		ft_putstr_fd("open failed", STDERR_FILENO);
		return(-1);
	}
	dup2(fd , STDOUT_FILENO);
	close(fd);
	return(0);
}

int handle_append_redirect(t_redirect *redir)
{
	int fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("open failed", STDERR_FILENO);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return(0);
}

int handle_heredoc(t_redirect *redir)
{
    int fd[2];
    char *line;
    
    if (pipe(fd) == -1)
        return (-1);
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, redir->file) == 0)
        {
            free(line);
            break;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    close(fd[1]);
    return (fd[0]);
}

int check_redirections(t_cmd *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN)
		{
			if (handle_input_redirect(redir) == -1)
				return (-1);
		}
		else if (redir->type == TOKEN_REDIR_OUT)
		{
			if (handle_output_redirect(redir) == -1)
				return (-1);
		}
		else if (redir->type == TOKEN_APPEND)
		{
			if (handle_append_redirect(redir) == -1)
				return (-1);
		}
		else if (redir->type == TOKEN_HEREDOC)
		{
			dup2(redir->heredoc_fd, STDIN_FILENO);
			close(redir->heredoc_fd);
		}
		redir = redir->next;
	}
	return (0);
}
