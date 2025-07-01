#include "minishell.h"

int handle_input_redirect(t_redirect *redir)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	if(fd < 0)
	{
			ft_putstr_fd("no such file or directory\n", STDERR_FILENO);
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

// int check_redirections(t_cmd *cmd)
// {
// 	t_redirect *redir = cmd->redirs;
	
// 	if(!cmd || !cmd->redirs)
// 		return(1);
// 	while(redir)
// 	{
// 		if(redir->type == TOKEN_REDIR_IN)
// 		{
// 			if(handle_input_redirect(redir) == -1)
// 				return(-1);
// 		}
// 		else if (redir->type == TOKEN_REDIR_OUT)
// 		{
// 			if(handle_output_redirect(redir) == -1)
// 				return(-1);
// 		}
// 		else if (redir->type == TOKEN_APPEND)
// 		{
// 			if(handle_append_redirect(redir) == -1)
// 				return(-1);
// 		}
// 		redir = redir->next;
// 	}
// 	return(0);
// }

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
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    return (0);
}

int check_redirections(t_cmd *cmd)
{
    t_redirect *redir = cmd->redirs;
    
    if(!cmd || !cmd->redirs)
        return(1);
    while(redir)
    {
        if(redir->type == TOKEN_REDIR_IN)
        {
            if(handle_input_redirect(redir) == -1)
                return(-1);
        }
        else if (redir->type == TOKEN_REDIR_OUT)
        {
            if(handle_output_redirect(redir) == -1)
                return(-1);
        }
        else if (redir->type == TOKEN_APPEND)
        {
            if(handle_append_redirect(redir) == -1)
                return(-1);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            if(handle_heredoc(redir) == -1)
                return(-1);
        }
        redir = redir->next;
    }
    return(0);
}