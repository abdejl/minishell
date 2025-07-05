#include "minishell.h"
#include <sys/wait.h>
#include <stdlib.h>

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
    {
          int i = 1;
          int has_error = 0;

          while (cmd->args[i])
          {
            if (!is_valid_identifier(cmd->args[i]))
            {
                is_valid_identifier(cmd->args[i]);
                has_error = 1;
            }
            else
                ft_unset(&shell->env_list, cmd->args[i]);
            i++;
          }
          shell->exit_status = has_error;
          return (0);
    }
    else if (!ft_strcmp(cmd->args[0], "env"))
        return (ft_env(shell->env_list));
    else if (!ft_strcmp(cmd->args[0], "exit"))
         return (mini_exit(shell, cmd));
    return (1);
}

char *get_cmd_path(char *cmd_name, t_env *env_list)
{
    char **paths;
    char *path_var = get_env_value(env_list, "PATH");
    char *full_path;
    int i = -1;
    
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

int exec_external(t_shell *shell, t_cmd *cmd)
{
    char **envp = env_to_array(shell->env_list);
    char *path = get_cmd_path(cmd->args[0], shell->env_list);
    int status = 127;

    if (!path) {
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);  // Just the command name
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
    } else {
        execve(path, cmd->args, envp);
        ft_putstr_fd("minishell: ", STDERR_FILENO);  // Keep prefix for execution errors
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putstr_fd(": execution failed\n", STDERR_FILENO);
        status = 126;
        free(path);
    }
    
    free_str_array(envp);
    return (status);
}

int handle_cmd(t_shell *shell, t_cmd *cmd)
{
    pid_t pid;
    int status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);
    if (is_builtin(cmd) && !cmd->next && !cmd->redirs)
    {
        shell->exit_status = exec_builtin(shell, cmd);
        return (shell->exit_status);
    }
    pid = fork();
    if (pid == 0)
    {
        set_signal_child();
        if (check_redirections(cmd) == -1)
            exit(1);
        if (is_builtin(cmd))
            exit(exec_builtin(shell, cmd));
        else
            exit(exec_external(shell, cmd));
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
        return (shell->exit_status);
    }
    perror("fork");
    return (1);
}

