#include "minishell.h"
# define TRUE 1
# define FALSE 0
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

int handle_input_redirect(t_redirect *redir)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	if(fd < 0)
	{
        ft_putstr_fd(redir->file, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
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
		ft_putstr_fd(" Permission denied", STDERR_FILENO);
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
		ft_putstr_fd(" Permission denied", STDERR_FILENO);
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
int	open_infile(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
int	open_outfile(const char *filename, int append)
{
	int	fd;

	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
static t_redirect	*get_last_redirects(t_cmd *cmd,
	t_redirect **in, t_redirect **out, t_redirect **heredoc)
{
	t_redirect	*r;

	*in = NULL;
	*out = NULL;
	*heredoc = NULL;
	r = cmd->redirs;
	while (r)
	{
		if (r->type == TOKEN_REDIR_IN)
			*in = r;
		else if (r->type == TOKEN_REDIR_OUT || r->type == TOKEN_APPEND)
			*out = r;
		else if (r->type == TOKEN_HEREDOC)
			*heredoc = r;
		r = r->next;
	}
	return (NULL);
}

int	check_redirections(t_cmd *cmd)
{
	t_redirect	*in;
	t_redirect	*out;
	t_redirect	*heredoc;

	get_last_redirects(cmd, &in, &out, &heredoc);
	if (in && open_infile(in->file) == -1)
		return (-1);
	if (heredoc && dup2(heredoc->heredoc_fd, STDIN_FILENO) == -1)
		return (perror("dup2"), -1);
	if (out)
	{
		if (out->type == TOKEN_APPEND)
		{
			if (open_outfile(out->file, TRUE) == -1)
				return (-1);
		}
		else if (open_outfile(out->file, FALSE) == -1)
			return (-1);
	}
	return (0);
}
