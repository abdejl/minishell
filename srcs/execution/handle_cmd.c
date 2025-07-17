#include "minishell.h"

void exec_child(t_shell *shell, t_cmd *cmd)
{
    set_signal_child();
    if (check_redirections(cmd) == -1)
        exit(1);
    if (is_builtin(cmd))
        exit(exec_builtin(shell, cmd));
    exit(exec_external(shell, cmd));
}

int wait_and_update_exit_status(pid_t pid, t_shell *shell)
{
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
    return (shell->exit_status);
}

int is_builtin(t_cmd *cmd)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);
    if (!ft_strcmp(cmd->args[0], "cd") ||
        !ft_strcmp(cmd->args[0], "echo") ||
        !ft_strcmp(cmd->args[0], "pwd") ||
        !ft_strcmp(cmd->args[0], "export") ||
        !ft_strcmp(cmd->args[0], "unset") ||
        !ft_strcmp(cmd->args[0], "env") ||
        !ft_strcmp(cmd->args[0], "exit"))
        return (1);
    return (0);
}

int exec_builtin(t_shell *shell, t_cmd *cmd)
{
    if (!ft_strcmp(cmd->args[0], "cd"))
        return (cd_builtin(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "echo"))
        return (ft_echo(cmd));
    else if (!ft_strcmp(cmd->args[0], "pwd"))
        return (ft_pwd());
    else if (!ft_strcmp(cmd->args[0], "export"))
        return (ft_export(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "unset"))
        return (ft_unset(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "env"))
        return (ft_env(shell->env_list));
    else if (!ft_strcmp(cmd->args[0], "exit"))
         return (mini_exit(shell, cmd));
    return (1);
}

char *get_cmd_path(char *cmd_name, t_env *env_list)
{
    char **paths;
    char *path_var;
    char *full_path;
    int i = -1;
    
    path_var = get_env_value(env_list, "PATH");
    if (!path_var)
        return (NULL);
    paths = ft_split(path_var, ':');
    if (!paths)
        return (NULL);
    while (paths[++i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = strjoin_free(full_path, cmd_name);
        if (access(full_path, X_OK) == 0)
        {
            free_str_array(paths);
            return (full_path);
        }
        free(full_path);
    }
    free_str_array(paths);
    return (NULL);
}

int	exec_external(t_shell *shell, t_cmd *cmd)
{
	char	**envp;
	char	*path;

	envp = env_to_array(shell->env_list);
	if (!cmd->args[0] || cmd->args[0][0] == '\0')
	{
		//ft_putstr_fd("minishell:", STDERR_FILENO);
		//ft_putendl_fd(":", STDERR_FILENO);
		free_str_array(envp);
		exit(0);
	}
	if (ft_strchr(cmd->args[0], '/'))
		path = ft_strdup(cmd->args[0]);
	else
		path = get_cmd_path(cmd->args[0], shell->env_list);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		free_str_array(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	if (access(path, F_OK) != 0)
	{
		perror(path);
		free(path);
		free_str_array(envp);
		exit(127);
	}
	else if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		free(path);
		free_str_array(envp);
		exit(126);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		free(path);
		free_str_array(envp);
		exit(126);
	}
}


// int exec_external(t_shell *shell, t_cmd *cmd)
// {
// 	char **envp = env_to_array(shell->env_list);
// 	char *path;

//     if (ft_strchr(cmd->args[0], '/'))
//         path = ft_strdup(cmd->args[0]);
//     else
//     {
//         path = get_cmd_path(cmd->args[0], shell->env_list);
//     }
// 	if (!path)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
// 		ft_putendl_fd(": command not found", STDERR_FILENO);
// 		free_str_array(envp);
// 		exit(127); 
// 	}
// 	execve(path, cmd->args, envp);
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
// 	ft_putendl_fd("command not found", STDERR_FILENO);
// 	free(path);
// 	free_str_array(envp);
// 	exit(126);
// }

int handle_cmd(t_shell *shell, t_cmd *cmd)
{
    pid_t pid;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);
    if (is_builtin(cmd) && !cmd->next && !cmd->redirs)
        return (exec_builtin(shell, cmd));
    pid = fork();
    if (pid == 0)
        exec_child(shell, cmd);
    else if (pid > 0)
        return wait_and_update_exit_status(pid, shell);
    perror("minishell");
    shell->exit_status = errno;
    return (shell->exit_status);
}

